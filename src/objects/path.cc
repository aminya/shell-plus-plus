// Copyright 2016 Alex Silva Torres
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "path.h"

#include <unistd.h>
#include <sys/stat.h>

#include "obj-type.h"
#include "object-factory.h"
#include "utils/check.h"

namespace shpp {
namespace internal {

PathObject::PathObject(const std::string& str_path, ObjectPtr obj_type,
    SymbolTableStack&& sym_table)
    : Object(ObjectType::PATH, obj_type, std::move(sym_table))
    , path_{str_path} {}

PathObject::PathObject(const boost::filesystem::path& path, ObjectPtr obj_type,
    SymbolTableStack&& sym_table)
    : Object(ObjectType::PATH, obj_type, std::move(sym_table))
    , path_{path} {}

ObjectPtr PathObject::ObjString() {
  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewString(path_.string());
}

ObjectPtr PathObject::ObjCmd() {
  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewString(path_.string());
}

boost::filesystem::path& PathObject::value() {
  return path_;
}

ObjectPtr PathObject::Attr(std::shared_ptr<Object> self,
                           const std::string& name) {
  ObjectPtr obj_type = ObjType();
  return static_cast<TypeObject&>(*obj_type).CallObject(name, self);
}

ObjectPtr PathObject::Equal(ObjectPtr obj) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_PARAM_TYPE(obj, equal, PATH)

  ObjectFactory obj_factory(symbol_table_stack());

  PathObject& path_obj = static_cast<PathObject&>(*obj);

  try {
    bool v = fs::equivalent(path_, path_obj.value());
    return obj_factory.NewBool(v);
  } catch (boost::filesystem::filesystem_error& ex) {
    throw RunTimeError(RunTimeError::ErrorCode::INVALID_ARGS,
        boost::format("Operation not permitted: not valid paths %1%, %2%")%
        path_ % path_obj.value());
  }
}

ObjectPtr PathObject::Div(ObjectPtr obj) {
  ObjectFactory obj_factory(symbol_table_stack());

  if (obj->type() == ObjectType::STRING) {
    const std::string& str_path = static_cast<StringObject&>(*obj).value();
    boost::filesystem::path tmp_path(str_path);
    boost::filesystem::path path = path_ / tmp_path;
    return obj_factory.NewPath(path);
  } else if (obj->type() == ObjectType::PATH) {
    const boost::filesystem::path& tmp_path =
        static_cast<PathObject&>(*obj).value();
    boost::filesystem::path path = path_ / tmp_path;
    return obj_factory.NewPath(path);
  } else {
    throw RunTimeError(RunTimeError::ErrorCode::INCOMPATIBLE_TYPE,
        boost::format("given argument must be string or path object"));
  }
}

PathType::PathType(ObjectPtr obj_type, SymbolTableStack&& sym_table)
    : TypeObject("path", obj_type, std::move(sym_table)) {
  RegisterStaticMethod<PathPwdStaticFunc>("pwd",  symbol_table_stack(), *this);
  RegisterMethod<PathExistsFunc>("exists", symbol_table_stack(), *this);
  RegisterMethod<PathIsRegularFileFunc>("is_regular_file",
                                        symbol_table_stack(), *this);
  RegisterMethod<PathIsDirFunc>("is_dir", symbol_table_stack(), *this);
  RegisterMethod<PathIsSymLinkFunc>("is_sym_link", symbol_table_stack(), *this);
  RegisterMethod<PathIsReadableFunc>("is_readable",
                                     symbol_table_stack(), *this);
  RegisterMethod<PathIsWritableFunc>("is_writable",
                                     symbol_table_stack(), *this);
  RegisterMethod<PathIsExecutableFunc>("is_exec", symbol_table_stack(), *this);
  RegisterMethod<PathOwnerUidFunc>("uid_owner", symbol_table_stack(), *this);
  RegisterMethod<PathOwnerGidFunc>("gid_owner", symbol_table_stack(), *this);
  RegisterMethod<PathRootNameFunc>("root_name", symbol_table_stack(), *this);
  RegisterMethod<PathRootDirectoryFunc>("root_dir",
                                        symbol_table_stack(), *this);
  RegisterMethod<PathRootPathFunc>("root_path", symbol_table_stack(), *this);
  RegisterMethod<PathRelativePathFunc>("relative_path",
                                       symbol_table_stack(), *this);
  RegisterMethod<PathParentPathFunc>("parent_path",
                                     symbol_table_stack(), *this);
  RegisterMethod<PathFilenameFunc>("filename", symbol_table_stack(), *this);
  RegisterMethod<PathStemFunc>("stem", symbol_table_stack(), *this);
  RegisterMethod<PathExtensionFunc>("extension", symbol_table_stack(), *this);
  RegisterMethod<PathAbsoluteFunc>("absolute", symbol_table_stack(), *this);
  RegisterMethod<PathSizeFunc>("size", symbol_table_stack(), *this);
}

ObjectPtr PathType::Constructor(Executor*, Args&& params, KWArgs&&) {
  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, path)
  SHPP_FUNC_CHECK_PARAM_TYPE(params[0], path, STRING)

  StringObject& str_obj = static_cast<StringObject&>(*params[0]);
  const std::string& str_path = str_obj.value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(str_path);
}

ObjectPtr PathType::Attr(std::shared_ptr<Object> /*self*/,
                         const std::string& name) {
  return this->CallStaticObject(name);
}

ObjectPtr PathPwdStaticFunc::Call(Executor*, Args&& params, KWArgs&&) {
  SHPP_FUNC_CHECK_NO_PARAMS(params, pwd)

  namespace fs = boost::filesystem;

  fs::path path = fs::current_path();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.string());
}

ObjectPtr PathExistsFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, exists)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewBool(fs::exists(path));
}

ObjectPtr PathIsRegularFileFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, exists)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewBool(fs::is_regular_file(path));
}

ObjectPtr PathIsDirFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, exists)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewBool(fs::is_directory(path));
}

ObjectPtr PathIsSymLinkFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, exists)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewBool(fs::is_symlink(path));
}

ObjectPtr PathIsReadableFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, is_readable)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());

  std::string str_path = path.string();
  if (access(str_path.c_str(), R_OK) < 0) {
    return obj_factory.NewBool(false);
  }

  return obj_factory.NewBool(true);
}

ObjectPtr PathIsWritableFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, is_readable)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());

  std::string str_path = path.string();
  if (access(str_path.c_str(), W_OK) < 0) {
   return obj_factory.NewBool(false);
  }

  return obj_factory.NewBool(true);
}

ObjectPtr PathIsExecutableFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, is_exec)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());

  std::string str_path = path.string();
  if (access(str_path.c_str(), X_OK) < 0) {
  return obj_factory.NewBool(false);
  }

  return obj_factory.NewBool(true);
}

ObjectPtr PathOwnerUidFunc::Call(Executor*, Args&& params, KWArgs&&) {
  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, owner_uid)

  namespace fs = boost::filesystem;

  struct stat sb;

  fs::path& path = static_cast<PathObject&>(*params[0]).value();
  std::string str_path = path.string();

  if (stat(str_path.c_str(), &sb) == -1) {
    throw RunTimeError(RunTimeError::ErrorCode::FILE,
                      boost::format("%1%")%strerror(errno));
  }

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewInt(sb.st_uid);
}

ObjectPtr PathOwnerGidFunc::Call(Executor*, Args&& params, KWArgs&&) {
  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, owner_gid)

  namespace fs = boost::filesystem;

  struct stat sb;

  fs::path& path = static_cast<PathObject&>(*params[0]).value();
  std::string str_path = path.string();

  if (stat(str_path.c_str(), &sb) == -1) {
  throw RunTimeError(RunTimeError::ErrorCode::FILE,
                     boost::format("%1%")%strerror(errno));
  }

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewInt(sb.st_gid);
}

ObjectPtr PathRootNameFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, root_name)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.root_name());
}

ObjectPtr PathRootDirectoryFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, root_dir)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.root_directory());
}

ObjectPtr PathRootPathFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, root_path)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.root_path());
}

ObjectPtr PathRelativePathFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, relative)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.relative_path());
}

ObjectPtr PathParentPathFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, parent_path)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.parent_path());
}

ObjectPtr PathFilenameFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, filename)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.filename());
}

ObjectPtr PathStemFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, stem)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.stem());
}

ObjectPtr PathExtensionFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, extensio)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(path.extension());
}

ObjectPtr PathAbsoluteFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS(params, 1, absolute)
  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewPath(canonical(path));
}

ObjectPtr PathSizeFunc::Call(Executor*, Args&& params, KWArgs&&) {
  namespace fs = boost::filesystem;

  SHPP_FUNC_CHECK_NUM_PARAMS_AT_LEAST(params, 1, size)
  SHPP_FUNC_CHECK_NUM_PARAMS_UNTIL(params, 2, size)

  fs::path& path = static_cast<PathObject&>(*params[0]).value();

  ObjectFactory obj_factory(symbol_table_stack());

  if (params.size() == 1) {
    int size = Size(path, false);
    return obj_factory.NewInt(size);
  }

  SHPP_FUNC_CHECK_PARAM_TYPE(params[1], type, STRING)

  // so params must have two elements
  size_t size = Size(path, false);
  std::string t = static_cast<StringObject&>(*params[1]).value();

  if (t == "k") {
    return obj_factory.NewReal(static_cast<float>(size/1024));
  } else if (t == "M") {
    return obj_factory.NewReal(static_cast<float>(size/(1024*1024)));
  } else if (t == "G") {
    return obj_factory.NewReal(static_cast<float>(size/(1024*1024*1024)));
  } else {
    return obj_factory.NewInt(size);
  }
}


size_t PathSizeFunc::Size(const boost::filesystem::path& path, bool rec) {
  namespace fs = boost::filesystem;

  try {
    if (fs::is_directory(path)) {
      size_t size = 0;

      for(auto& entry : boost::make_iterator_range(
          fs::directory_iterator(path), {})) {
        size += Size(entry, true);
      }

      return size;
    } else if (fs::is_regular(path)) {
      return fs::file_size(path);
    } else if (!fs::exists(path) && !rec) {
      throw RunTimeError(RunTimeError::ErrorCode::FILE,
          boost::format("file: '%1%' not exists")%path);
    } else {
      return 0;
    }
  } catch (const fs::filesystem_error& ex) {
      throw RunTimeError(RunTimeError::ErrorCode::FILE,
          boost::format("%1%")%ex.what());
  }
}

}
}
