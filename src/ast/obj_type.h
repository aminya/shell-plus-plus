#ifndef SETI_OBJ_TYPE_H
#define SETI_OBJ_TYPE_H

#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>

namespace setti {
namespace internal {

class EntryPointer {
 public:
  enum class EntryType: uint8_t {
    SYMBOL,
    OBJECT
  };

  EntryType entry_type() const noexcept {
    return type_;
  }

  EntryPointer(const EntryPointer& other): type_(other.type_) {}

 protected:
  EntryPointer(EntryType type): type_(type) {}

 private:
  EntryType type_;
};

class Object: public EntryPointer {
 public:
  enum class ObjectType: uint8_t {
    NIL,
    INT,
    BOOL,
    REAL,
    STRING,
    ARRAY,
    MAP,
    TUPLE,
    CUSTON
  };

  Object(const Object& obj): EntryPointer(obj), type_(obj.type_) {}

  virtual ~Object() {}

  inline ObjectType type() {
    return type_;
  }

  virtual void Print() = 0;

 private:
  ObjectType type_;

 protected:
  Object(ObjectType type)
      : EntryPointer(EntryPointer::EntryType::OBJECT), type_(type) {}
};

typedef std::shared_ptr<Object> ObjectPtr;

class NullObject: public Object {
 public:
  NullObject(): Object(ObjectType::NIL) {}
  virtual ~NullObject() {}

  void Print() override {
    std::cout << "NIL";
  }

  inline nullptr_t value() const noexcept { return nullptr; }
};

class IntObject: public Object {
 public:
  IntObject(int value): Object(ObjectType::INT), value_(value) {}
  IntObject(const IntObject& obj): Object(obj), value_(obj.value_) {}
  virtual ~IntObject() {}

  IntObject& operator=(const IntObject& obj) {
    value_ = obj.value_;
    return *this;
  }

  inline int value() const noexcept { return value_; }

  void Print() override {
    std::cout << "INT: " << value_;
  }

 private:
  int value_;
};

class BoolObject: public Object {
 public:
  BoolObject(bool value): Object(ObjectType::BOOL), value_(value) {}
  BoolObject(const BoolObject& obj): Object(obj), value_(obj.value_) {}
  virtual ~BoolObject() {}

  BoolObject& operator=(const BoolObject& obj) {
    value_ = obj.value_;
    return *this;
  }

  inline bool value() const noexcept { return value_; }

  void Print() override {
    std::cout << "BOOL: " << value_;
  }

 private:
  bool value_;
};

class RealObject: public Object {
 public:
  RealObject(float value): Object(ObjectType::REAL), value_(value) {}
  RealObject(const RealObject& obj): Object(obj), value_(obj.value_) {}
  virtual ~RealObject() {}

  RealObject& operator=(const RealObject& obj) {
    value_ = obj.value_;
    return *this;
  }

  inline float value() const noexcept { return value_; }

  void Print() override {
    std::cout << "REAL: " << value_;
  }

 private:
  float value_;
};

class StringObject: public Object {
 public:
  StringObject(std::string&& value)
      : Object(ObjectType::STRING), value_(std::move(value)) {}
  StringObject(const StringObject& obj): Object(obj), value_(obj.value_) {}

  virtual ~StringObject() {}

  StringObject& operator=(const StringObject& obj) {
    value_ = obj.value_;
    return *this;
  }

  inline const std::string& value() const noexcept { return value_; }

  void Print() override {
    std::cout << "STRING: " << value_;
  }

 private:
  std::string value_;
};

class TupleObject: public Object {
 public:
   TupleObject(std::vector<std::unique_ptr<Object>>&& value)
      : Object(ObjectType::TUPLE), value_(value.size()) {
     for (size_t i = 0; i < value.size(); i++) {
       Object* obj_ptr = value[i].release();
       value_[i] = std::shared_ptr<Object>(obj_ptr);
     }
   }

   TupleObject(std::vector<std::shared_ptr<Object>>&& value)
      : Object(ObjectType::TUPLE), value_(value) {}

   TupleObject(const TupleObject& obj): Object(obj), value_(obj.value_) {}

   virtual ~TupleObject() {}

   inline std::shared_ptr<Object>& ElementRef(size_t i) {
     return value_.at(i);
   }

   inline std::shared_ptr<Object> Element(size_t i) {
     return value_.at(i);
   }

   inline void set(size_t i, std::unique_ptr<Object> obj) {
     Object* obj_ptr = obj.release();
     value_[i] = std::shared_ptr<Object>(obj_ptr);
   }

   void Print() override {
     std::cout << "TUPLE: ( ";
     for (const auto& e: value_) {
       e->Print();
       std::cout << " ";
     }
     std::cout << ")";
   }

 private:
  std::vector<std::shared_ptr<Object>> value_;
};

class ArrayObject: public Object {
 public:
   ArrayObject(std::vector<std::unique_ptr<Object>>&& value)
      : Object(ObjectType::ARRAY), value_(value.size()) {
     for (size_t i = 0; i < value.size(); i++) {
       Object* obj_ptr = value[i].release();
       value_[i] = std::shared_ptr<Object>(obj_ptr);
     }
   }

   ArrayObject(std::vector<std::shared_ptr<Object>>&& value)
      : Object(ObjectType::ARRAY), value_(value) {}

   ArrayObject(const ArrayObject& obj): Object(obj), value_(obj.value_) {}

   virtual ~ArrayObject() {}

   inline Object* at(size_t i) {
     return value_.at(i).get();
   }

   inline std::shared_ptr<Object>& ElementRef(size_t i) {
     return value_.at(i);
   }

   inline std::shared_ptr<Object> Element(size_t i) {
     return value_.at(i);
   }

   inline void set(size_t i, std::unique_ptr<Object> obj) {
     Object* obj_ptr = obj.release();
     value_[i] = std::shared_ptr<Object>(obj_ptr);
   }

   void Print() override {
     std::cout << "ARRAY: [ ";
     for (const auto& e: value_) {
       e->Print();
       std::cout << " ";
     }
     std::cout << "]";
   }

 private:
  std::vector<std::shared_ptr<Object>> value_;
};

}
}

#endif  // SETI_OBJ_TYPE_H

