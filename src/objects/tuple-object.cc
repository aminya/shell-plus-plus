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

#include "tuple-object.h"

#include "simple-object.h"
#include "object-factory.h"

namespace shpp {
namespace internal {

TupleIterObject::TupleIterObject(ObjectPtr tuple_obj, ObjectPtr obj_type,
                                 SymbolTableStack&& sym_table)
    : BaseIter(ObjectType::TUPLE_ITER, obj_type, std::move(sym_table))
    , pos_(0) {
  if (tuple_obj->type() != ObjectType::TUPLE) {
    throw RunTimeError(RunTimeError::ErrorCode::INCOMPATIBLE_TYPE,
                       boost::format("argument must be tuple"));
  }

  tuple_obj_ = tuple_obj;
}

ObjectPtr TupleIterObject::Equal(ObjectPtr obj) {
  ObjectFactory obj_factory(symbol_table_stack());

  if (obj->type() != ObjectType::ARRAY_ITER) {
    return obj_factory.NewBool(false);
  }

  TupleIterObject& other = static_cast<TupleIterObject&>(*obj);

  bool ptr_eq = obj.get() == tuple_obj_.get();
  bool pos_eq = other.pos_ == pos_;

  return obj_factory.NewBool(ptr_eq && pos_eq);
}

ObjectPtr TupleIterObject::Next() {
  TupleObject& tuple_obj = static_cast<TupleObject&>(*tuple_obj_);

  if (pos_ >= tuple_obj.Size()) {
    ObjectFactory obj_factory(symbol_table_stack());
    return obj_factory.NewNull();
  }

  return tuple_obj.Element(pos_++);
}

ObjectPtr TupleIterObject::HasNext() {
  ObjectFactory obj_factory(symbol_table_stack());

  bool v = pos_ == static_cast<TupleObject&>(*tuple_obj_).Size();
  return obj_factory.NewBool(!v);
}

ObjectPtr TupleIterType::Constructor(Executor*, Args&& params, KWArgs&&) {
  if (params.size() != 1) {
    throw RunTimeError(RunTimeError::ErrorCode::FUNC_PARAMS,
                       boost::format("tuple_iter() takes exactly 1 argument"));
  }

  if (params[0]->type() != ObjectType::TUPLE) {
    throw RunTimeError(RunTimeError::ErrorCode::INCOMPATIBLE_TYPE,
                       boost::format("invalid type for tuple_iter"));
  }

  ObjectFactory obj_factory(symbol_table_stack());
  ObjectPtr obj(obj_factory.NewTupleIter(params[0]));
  return obj;
}

std::size_t TupleObject::Hash() {
  if (value_.empty()) {
    throw RunTimeError(RunTimeError::ErrorCode::OUT_OF_RANGE,
                       boost::format("hash of empty tuple is not valid"));
  }

  size_t hash = 0;

  // Executes xor operation with hash of each element of tuple
  for (auto& e: value_) {
    hash ^= e->Hash();
  }

  return hash;
}

bool TupleObject::operator==(const Object& obj) {
  if (obj.type() != ObjectType::TUPLE) {
    return false;
  }

  const TupleObject& tuple_obj = static_cast<const TupleObject&>(obj);

  // If the tuples have different size, they are different
  if (tuple_obj.value_.size() != value_.size()) {
    return false;
  }

  bool r = true;

  // Test each element on tuple
  for (size_t i = 0; i < value_.size(); i++) {
    r = r && (tuple_obj.value_[i] == value_[i]);
  }

  return r;
}

ObjectPtr TupleObject::Element(const SliceObject& slice) {
  int start = 0;
  int end = value_.size();
  int step = 1;

  std::tie(start, end, step) = SliceLogic(slice, value_.size());

  std::vector<std::shared_ptr<Object>> values;
  for (int i = start; i < end; i += step) {
    values.push_back(value_[i]);
  }

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewArray(std::move(values));
}

ObjectPtr TupleObject::ObjIter(ObjectPtr obj) {
  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewTupleIter(obj);
}

ObjectPtr TupleObject::Copy() {
  ObjectFactory obj_factory(symbol_table_stack());
  std::vector<std::shared_ptr<Object>> value = value_;
  return obj_factory.NewTuple(std::move(value));
}

ObjectPtr TupleObject::GetItem(ObjectPtr index) {
  if (index->type() == ObjectType::SLICE) {
    return Element(static_cast<SliceObject&>(*index));
  } else if (index->type() == ObjectType::INT) {
    return Element(static_cast<IntObject&>(*index).value());
  } else {
    throw RunTimeError(RunTimeError::ErrorCode::INCOMPATIBLE_TYPE,
                       boost::format("index type not valid"));
  }
}

ObjectPtr& TupleObject::GetItemRef(ObjectPtr index) {
  if (index->type() != ObjectType::INT) {
    throw RunTimeError(RunTimeError::ErrorCode::INCOMPATIBLE_TYPE,
                       boost::format("index type not valid"));
  }

  return ElementRef(static_cast<IntObject&>(*index).value());
}

void TupleObject::SetItem(std::shared_ptr<Object> index,
    std::shared_ptr<Object> value) {
  if (index->type() != ObjectType::INT) {
    throw RunTimeError(RunTimeError::ErrorCode::INCOMPATIBLE_TYPE,
                       boost::format("index type not valid"));
  }

  int num_index = static_cast<IntObject&>(*index).value();

  if (num_index >= static_cast<int>(value_.size()) ||  num_index < 0) {
    throw RunTimeError(RunTimeError::ErrorCode::OUT_OF_RANGE,
                       boost::format("value: %1% out of range of tuple")
                       %num_index);
  }

  value_[num_index] = value;
}

ObjectPtr TupleObject::ObjArray() {
  std::vector<ObjectPtr> values = value_;

  ObjectFactory obj_factory(symbol_table_stack());
  return obj_factory.NewArray(std::move(values));
}

}
}
