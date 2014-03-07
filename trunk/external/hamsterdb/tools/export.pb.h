// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: export.proto

#ifndef PROTOBUF_export_2eproto__INCLUDED
#define PROTOBUF_export_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace HamsterTool {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_export_2eproto();
void protobuf_AssignDesc_export_2eproto();
void protobuf_ShutdownFile_export_2eproto();

class Datum;
class Environment;
class Database;
class Item;

enum Datum_Type {
  Datum_Type_ENVIRONMENT = 1,
  Datum_Type_DATABASE = 2,
  Datum_Type_ITEM = 3
};
bool Datum_Type_IsValid(int value);
const Datum_Type Datum_Type_Type_MIN = Datum_Type_ENVIRONMENT;
const Datum_Type Datum_Type_Type_MAX = Datum_Type_ITEM;
const int Datum_Type_Type_ARRAYSIZE = Datum_Type_Type_MAX + 1;

// ===================================================================

class Datum : public ::google::protobuf::MessageLite {
 public:
  Datum();
  virtual ~Datum();
  
  Datum(const Datum& from);
  
  inline Datum& operator=(const Datum& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const Datum& default_instance();
  
  void Swap(Datum* other);
  
  // implements Message ----------------------------------------------
  
  Datum* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const Datum& from);
  void MergeFrom(const Datum& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  typedef Datum_Type Type;
  static const Type ENVIRONMENT = Datum_Type_ENVIRONMENT;
  static const Type DATABASE = Datum_Type_DATABASE;
  static const Type ITEM = Datum_Type_ITEM;
  static inline bool Type_IsValid(int value) {
    return Datum_Type_IsValid(value);
  }
  static const Type Type_MIN =
    Datum_Type_Type_MIN;
  static const Type Type_MAX =
    Datum_Type_Type_MAX;
  static const int Type_ARRAYSIZE =
    Datum_Type_Type_ARRAYSIZE;
  
  // accessors -------------------------------------------------------
  
  // required .HamsterTool.Datum.Type type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::HamsterTool::Datum_Type type() const;
  inline void set_type(::HamsterTool::Datum_Type value);
  
  // optional .HamsterTool.Environment env = 2;
  inline bool has_env() const;
  inline void clear_env();
  static const int kEnvFieldNumber = 2;
  inline const ::HamsterTool::Environment& env() const;
  inline ::HamsterTool::Environment* mutable_env();
  inline ::HamsterTool::Environment* release_env();
  
  // optional .HamsterTool.Database db = 3;
  inline bool has_db() const;
  inline void clear_db();
  static const int kDbFieldNumber = 3;
  inline const ::HamsterTool::Database& db() const;
  inline ::HamsterTool::Database* mutable_db();
  inline ::HamsterTool::Database* release_db();
  
  // optional .HamsterTool.Item item = 4;
  inline bool has_item() const;
  inline void clear_item();
  static const int kItemFieldNumber = 4;
  inline const ::HamsterTool::Item& item() const;
  inline ::HamsterTool::Item* mutable_item();
  inline ::HamsterTool::Item* release_item();
  
  // @@protoc_insertion_point(class_scope:HamsterTool.Datum)
 private:
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_env();
  inline void clear_has_env();
  inline void set_has_db();
  inline void clear_has_db();
  inline void set_has_item();
  inline void clear_has_item();
  
  ::HamsterTool::Environment* env_;
  ::HamsterTool::Database* db_;
  ::HamsterTool::Item* item_;
  int type_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_export_2eproto();
  friend void protobuf_AssignDesc_export_2eproto();
  friend void protobuf_ShutdownFile_export_2eproto();
  
  void InitAsDefaultInstance();
  static Datum* default_instance_;
};
// -------------------------------------------------------------------

class Environment : public ::google::protobuf::MessageLite {
 public:
  Environment();
  virtual ~Environment();
  
  Environment(const Environment& from);
  
  inline Environment& operator=(const Environment& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const Environment& default_instance();
  
  void Swap(Environment* other);
  
  // implements Message ----------------------------------------------
  
  Environment* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const Environment& from);
  void MergeFrom(const Environment& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 flags = 1;
  inline bool has_flags() const;
  inline void clear_flags();
  static const int kFlagsFieldNumber = 1;
  inline ::google::protobuf::uint32 flags() const;
  inline void set_flags(::google::protobuf::uint32 value);
  
  // required uint32 pagesize = 2;
  inline bool has_pagesize() const;
  inline void clear_pagesize();
  static const int kPagesizeFieldNumber = 2;
  inline ::google::protobuf::uint32 pagesize() const;
  inline void set_pagesize(::google::protobuf::uint32 value);
  
  // required uint32 max_databases = 3;
  inline bool has_max_databases() const;
  inline void clear_max_databases();
  static const int kMaxDatabasesFieldNumber = 3;
  inline ::google::protobuf::uint32 max_databases() const;
  inline void set_max_databases(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:HamsterTool.Environment)
 private:
  inline void set_has_flags();
  inline void clear_has_flags();
  inline void set_has_pagesize();
  inline void clear_has_pagesize();
  inline void set_has_max_databases();
  inline void clear_has_max_databases();
  
  ::google::protobuf::uint32 flags_;
  ::google::protobuf::uint32 pagesize_;
  ::google::protobuf::uint32 max_databases_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_export_2eproto();
  friend void protobuf_AssignDesc_export_2eproto();
  friend void protobuf_ShutdownFile_export_2eproto();
  
  void InitAsDefaultInstance();
  static Environment* default_instance_;
};
// -------------------------------------------------------------------

class Database : public ::google::protobuf::MessageLite {
 public:
  Database();
  virtual ~Database();
  
  Database(const Database& from);
  
  inline Database& operator=(const Database& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const Database& default_instance();
  
  void Swap(Database* other);
  
  // implements Message ----------------------------------------------
  
  Database* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const Database& from);
  void MergeFrom(const Database& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required uint32 name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline ::google::protobuf::uint32 name() const;
  inline void set_name(::google::protobuf::uint32 value);
  
  // required uint32 flags = 2;
  inline bool has_flags() const;
  inline void clear_flags();
  static const int kFlagsFieldNumber = 2;
  inline ::google::protobuf::uint32 flags() const;
  inline void set_flags(::google::protobuf::uint32 value);
  
  // required uint32 keysize = 3;
  inline bool has_keysize() const;
  inline void clear_keysize();
  static const int kKeysizeFieldNumber = 3;
  inline ::google::protobuf::uint32 keysize() const;
  inline void set_keysize(::google::protobuf::uint32 value);
  
  // @@protoc_insertion_point(class_scope:HamsterTool.Database)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_flags();
  inline void clear_has_flags();
  inline void set_has_keysize();
  inline void clear_has_keysize();
  
  ::google::protobuf::uint32 name_;
  ::google::protobuf::uint32 flags_;
  ::google::protobuf::uint32 keysize_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_export_2eproto();
  friend void protobuf_AssignDesc_export_2eproto();
  friend void protobuf_ShutdownFile_export_2eproto();
  
  void InitAsDefaultInstance();
  static Database* default_instance_;
};
// -------------------------------------------------------------------

class Item : public ::google::protobuf::MessageLite {
 public:
  Item();
  virtual ~Item();
  
  Item(const Item& from);
  
  inline Item& operator=(const Item& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const Item& default_instance();
  
  void Swap(Item* other);
  
  // implements Message ----------------------------------------------
  
  Item* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const Item& from);
  void MergeFrom(const Item& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required bytes key = 1;
  inline bool has_key() const;
  inline void clear_key();
  static const int kKeyFieldNumber = 1;
  inline const ::std::string& key() const;
  inline void set_key(const ::std::string& value);
  inline void set_key(const char* value);
  inline void set_key(const void* value, size_t size);
  inline ::std::string* mutable_key();
  inline ::std::string* release_key();
  
  // optional bytes record = 2;
  inline bool has_record() const;
  inline void clear_record();
  static const int kRecordFieldNumber = 2;
  inline const ::std::string& record() const;
  inline void set_record(const ::std::string& value);
  inline void set_record(const char* value);
  inline void set_record(const void* value, size_t size);
  inline ::std::string* mutable_record();
  inline ::std::string* release_record();
  
  // @@protoc_insertion_point(class_scope:HamsterTool.Item)
 private:
  inline void set_has_key();
  inline void clear_has_key();
  inline void set_has_record();
  inline void clear_has_record();
  
  ::std::string* key_;
  ::std::string* record_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_export_2eproto();
  friend void protobuf_AssignDesc_export_2eproto();
  friend void protobuf_ShutdownFile_export_2eproto();
  
  void InitAsDefaultInstance();
  static Item* default_instance_;
};
// ===================================================================


// ===================================================================

// Datum

// required .HamsterTool.Datum.Type type = 1;
inline bool Datum::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Datum::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Datum::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Datum::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline ::HamsterTool::Datum_Type Datum::type() const {
  return static_cast< ::HamsterTool::Datum_Type >(type_);
}
inline void Datum::set_type(::HamsterTool::Datum_Type value) {
  GOOGLE_DCHECK(::HamsterTool::Datum_Type_IsValid(value));
  set_has_type();
  type_ = value;
}

// optional .HamsterTool.Environment env = 2;
inline bool Datum::has_env() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Datum::set_has_env() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Datum::clear_has_env() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Datum::clear_env() {
  if (env_ != NULL) env_->::HamsterTool::Environment::Clear();
  clear_has_env();
}
inline const ::HamsterTool::Environment& Datum::env() const {
  return env_ != NULL ? *env_ : *default_instance_->env_;
}
inline ::HamsterTool::Environment* Datum::mutable_env() {
  set_has_env();
  if (env_ == NULL) env_ = new ::HamsterTool::Environment;
  return env_;
}
inline ::HamsterTool::Environment* Datum::release_env() {
  clear_has_env();
  ::HamsterTool::Environment* temp = env_;
  env_ = NULL;
  return temp;
}

// optional .HamsterTool.Database db = 3;
inline bool Datum::has_db() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Datum::set_has_db() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Datum::clear_has_db() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Datum::clear_db() {
  if (db_ != NULL) db_->::HamsterTool::Database::Clear();
  clear_has_db();
}
inline const ::HamsterTool::Database& Datum::db() const {
  return db_ != NULL ? *db_ : *default_instance_->db_;
}
inline ::HamsterTool::Database* Datum::mutable_db() {
  set_has_db();
  if (db_ == NULL) db_ = new ::HamsterTool::Database;
  return db_;
}
inline ::HamsterTool::Database* Datum::release_db() {
  clear_has_db();
  ::HamsterTool::Database* temp = db_;
  db_ = NULL;
  return temp;
}

// optional .HamsterTool.Item item = 4;
inline bool Datum::has_item() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Datum::set_has_item() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Datum::clear_has_item() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Datum::clear_item() {
  if (item_ != NULL) item_->::HamsterTool::Item::Clear();
  clear_has_item();
}
inline const ::HamsterTool::Item& Datum::item() const {
  return item_ != NULL ? *item_ : *default_instance_->item_;
}
inline ::HamsterTool::Item* Datum::mutable_item() {
  set_has_item();
  if (item_ == NULL) item_ = new ::HamsterTool::Item;
  return item_;
}
inline ::HamsterTool::Item* Datum::release_item() {
  clear_has_item();
  ::HamsterTool::Item* temp = item_;
  item_ = NULL;
  return temp;
}

// -------------------------------------------------------------------

// Environment

// required uint32 flags = 1;
inline bool Environment::has_flags() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Environment::set_has_flags() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Environment::clear_has_flags() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Environment::clear_flags() {
  flags_ = 0u;
  clear_has_flags();
}
inline ::google::protobuf::uint32 Environment::flags() const {
  return flags_;
}
inline void Environment::set_flags(::google::protobuf::uint32 value) {
  set_has_flags();
  flags_ = value;
}

// required uint32 pagesize = 2;
inline bool Environment::has_pagesize() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Environment::set_has_pagesize() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Environment::clear_has_pagesize() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Environment::clear_pagesize() {
  pagesize_ = 0u;
  clear_has_pagesize();
}
inline ::google::protobuf::uint32 Environment::pagesize() const {
  return pagesize_;
}
inline void Environment::set_pagesize(::google::protobuf::uint32 value) {
  set_has_pagesize();
  pagesize_ = value;
}

// required uint32 max_databases = 3;
inline bool Environment::has_max_databases() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Environment::set_has_max_databases() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Environment::clear_has_max_databases() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Environment::clear_max_databases() {
  max_databases_ = 0u;
  clear_has_max_databases();
}
inline ::google::protobuf::uint32 Environment::max_databases() const {
  return max_databases_;
}
inline void Environment::set_max_databases(::google::protobuf::uint32 value) {
  set_has_max_databases();
  max_databases_ = value;
}

// -------------------------------------------------------------------

// Database

// required uint32 name = 1;
inline bool Database::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Database::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Database::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Database::clear_name() {
  name_ = 0u;
  clear_has_name();
}
inline ::google::protobuf::uint32 Database::name() const {
  return name_;
}
inline void Database::set_name(::google::protobuf::uint32 value) {
  set_has_name();
  name_ = value;
}

// required uint32 flags = 2;
inline bool Database::has_flags() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Database::set_has_flags() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Database::clear_has_flags() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Database::clear_flags() {
  flags_ = 0u;
  clear_has_flags();
}
inline ::google::protobuf::uint32 Database::flags() const {
  return flags_;
}
inline void Database::set_flags(::google::protobuf::uint32 value) {
  set_has_flags();
  flags_ = value;
}

// required uint32 keysize = 3;
inline bool Database::has_keysize() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Database::set_has_keysize() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Database::clear_has_keysize() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Database::clear_keysize() {
  keysize_ = 0u;
  clear_has_keysize();
}
inline ::google::protobuf::uint32 Database::keysize() const {
  return keysize_;
}
inline void Database::set_keysize(::google::protobuf::uint32 value) {
  set_has_keysize();
  keysize_ = value;
}

// -------------------------------------------------------------------

// Item

// required bytes key = 1;
inline bool Item::has_key() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Item::set_has_key() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Item::clear_has_key() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Item::clear_key() {
  if (key_ != &::google::protobuf::internal::kEmptyString) {
    key_->clear();
  }
  clear_has_key();
}
inline const ::std::string& Item::key() const {
  return *key_;
}
inline void Item::set_key(const ::std::string& value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void Item::set_key(const char* value) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(value);
}
inline void Item::set_key(const void* value, size_t size) {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  key_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Item::mutable_key() {
  set_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    key_ = new ::std::string;
  }
  return key_;
}
inline ::std::string* Item::release_key() {
  clear_has_key();
  if (key_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = key_;
    key_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// optional bytes record = 2;
inline bool Item::has_record() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Item::set_has_record() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Item::clear_has_record() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Item::clear_record() {
  if (record_ != &::google::protobuf::internal::kEmptyString) {
    record_->clear();
  }
  clear_has_record();
}
inline const ::std::string& Item::record() const {
  return *record_;
}
inline void Item::set_record(const ::std::string& value) {
  set_has_record();
  if (record_ == &::google::protobuf::internal::kEmptyString) {
    record_ = new ::std::string;
  }
  record_->assign(value);
}
inline void Item::set_record(const char* value) {
  set_has_record();
  if (record_ == &::google::protobuf::internal::kEmptyString) {
    record_ = new ::std::string;
  }
  record_->assign(value);
}
inline void Item::set_record(const void* value, size_t size) {
  set_has_record();
  if (record_ == &::google::protobuf::internal::kEmptyString) {
    record_ = new ::std::string;
  }
  record_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Item::mutable_record() {
  set_has_record();
  if (record_ == &::google::protobuf::internal::kEmptyString) {
    record_ = new ::std::string;
  }
  return record_;
}
inline ::std::string* Item::release_record() {
  clear_has_record();
  if (record_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = record_;
    record_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace HamsterTool

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_export_2eproto__INCLUDED