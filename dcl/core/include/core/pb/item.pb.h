// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: item.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_item_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_item_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_item_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_item_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_item_2eproto;
class PBItemData;
class PBItemDataDefaultTypeInternal;
extern PBItemDataDefaultTypeInternal _PBItemData_default_instance_;
class PBItemsDict;
class PBItemsDictDefaultTypeInternal;
extern PBItemsDictDefaultTypeInternal _PBItemsDict_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::PBItemData* Arena::CreateMaybeMessage<::PBItemData>(Arena*);
template<> ::PBItemsDict* Arena::CreateMaybeMessage<::PBItemsDict>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

enum PBItemEnum : int {
  STICK = 0,
  PEBBLE = 1,
  ROCK = 2,
  DIRT = 3,
  PUNCH = 1000,
  KICK = 1001,
  WOODEN_SWORD = 2000,
  WOODEN_HELMET = 3000,
  PBItemEnum_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  PBItemEnum_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool PBItemEnum_IsValid(int value);
constexpr PBItemEnum PBItemEnum_MIN = STICK;
constexpr PBItemEnum PBItemEnum_MAX = WOODEN_HELMET;
constexpr int PBItemEnum_ARRAYSIZE = PBItemEnum_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PBItemEnum_descriptor();
template<typename T>
inline const std::string& PBItemEnum_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PBItemEnum>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PBItemEnum_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PBItemEnum_descriptor(), enum_t_value);
}
inline bool PBItemEnum_Parse(
    const std::string& name, PBItemEnum* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PBItemEnum>(
    PBItemEnum_descriptor(), name, value);
}
enum PBItemType : int {
  RESOURCES = 0,
  ATTACKS = 1,
  WEAPONS = 2,
  EQUIPMENT = 3,
  QUEST_ITEMS = 4,
  PBItemType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  PBItemType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool PBItemType_IsValid(int value);
constexpr PBItemType PBItemType_MIN = RESOURCES;
constexpr PBItemType PBItemType_MAX = QUEST_ITEMS;
constexpr int PBItemType_ARRAYSIZE = PBItemType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PBItemType_descriptor();
template<typename T>
inline const std::string& PBItemType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PBItemType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PBItemType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PBItemType_descriptor(), enum_t_value);
}
inline bool PBItemType_Parse(
    const std::string& name, PBItemType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PBItemType>(
    PBItemType_descriptor(), name, value);
}
// ===================================================================

class PBItemData PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PBItemData) */ {
 public:
  inline PBItemData() : PBItemData(nullptr) {};
  virtual ~PBItemData();

  PBItemData(const PBItemData& from);
  PBItemData(PBItemData&& from) noexcept
    : PBItemData() {
    *this = ::std::move(from);
  }

  inline PBItemData& operator=(const PBItemData& from) {
    CopyFrom(from);
    return *this;
  }
  inline PBItemData& operator=(PBItemData&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const PBItemData& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PBItemData* internal_default_instance() {
    return reinterpret_cast<const PBItemData*>(
               &_PBItemData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(PBItemData& a, PBItemData& b) {
    a.Swap(&b);
  }
  inline void Swap(PBItemData* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PBItemData* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline PBItemData* New() const final {
    return CreateMaybeMessage<PBItemData>(nullptr);
  }

  PBItemData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<PBItemData>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const PBItemData& from);
  void MergeFrom(const PBItemData& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PBItemData* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PBItemData";
  }
  protected:
  explicit PBItemData(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_item_2eproto);
    return ::descriptor_table_item_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kImagePathFieldNumber = 1,
    kItemNameFieldNumber = 2,
    kItemIdFieldNumber = 3,
    kQuantityFieldNumber = 4,
  };
  // string image_path = 1;
  void clear_image_path();
  const std::string& image_path() const;
  void set_image_path(const std::string& value);
  void set_image_path(std::string&& value);
  void set_image_path(const char* value);
  void set_image_path(const char* value, size_t size);
  std::string* mutable_image_path();
  std::string* release_image_path();
  void set_allocated_image_path(std::string* image_path);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_image_path();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_image_path(
      std::string* image_path);
  private:
  const std::string& _internal_image_path() const;
  void _internal_set_image_path(const std::string& value);
  std::string* _internal_mutable_image_path();
  public:

  // string item_name = 2;
  void clear_item_name();
  const std::string& item_name() const;
  void set_item_name(const std::string& value);
  void set_item_name(std::string&& value);
  void set_item_name(const char* value);
  void set_item_name(const char* value, size_t size);
  std::string* mutable_item_name();
  std::string* release_item_name();
  void set_allocated_item_name(std::string* item_name);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_item_name();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_item_name(
      std::string* item_name);
  private:
  const std::string& _internal_item_name() const;
  void _internal_set_item_name(const std::string& value);
  std::string* _internal_mutable_item_name();
  public:

  // .PBItemEnum item_id = 3;
  void clear_item_id();
  ::PBItemEnum item_id() const;
  void set_item_id(::PBItemEnum value);
  private:
  ::PBItemEnum _internal_item_id() const;
  void _internal_set_item_id(::PBItemEnum value);
  public:

  // int32 quantity = 4;
  void clear_quantity();
  ::PROTOBUF_NAMESPACE_ID::int32 quantity() const;
  void set_quantity(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_quantity() const;
  void _internal_set_quantity(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:PBItemData)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr image_path_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr item_name_;
  int item_id_;
  ::PROTOBUF_NAMESPACE_ID::int32 quantity_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_item_2eproto;
};
// -------------------------------------------------------------------

class PBItemsDict PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PBItemsDict) */ {
 public:
  inline PBItemsDict() : PBItemsDict(nullptr) {};
  virtual ~PBItemsDict();

  PBItemsDict(const PBItemsDict& from);
  PBItemsDict(PBItemsDict&& from) noexcept
    : PBItemsDict() {
    *this = ::std::move(from);
  }

  inline PBItemsDict& operator=(const PBItemsDict& from) {
    CopyFrom(from);
    return *this;
  }
  inline PBItemsDict& operator=(PBItemsDict&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const PBItemsDict& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PBItemsDict* internal_default_instance() {
    return reinterpret_cast<const PBItemsDict*>(
               &_PBItemsDict_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(PBItemsDict& a, PBItemsDict& b) {
    a.Swap(&b);
  }
  inline void Swap(PBItemsDict* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PBItemsDict* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline PBItemsDict* New() const final {
    return CreateMaybeMessage<PBItemsDict>(nullptr);
  }

  PBItemsDict* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<PBItemsDict>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const PBItemsDict& from);
  void MergeFrom(const PBItemsDict& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PBItemsDict* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PBItemsDict";
  }
  protected:
  explicit PBItemsDict(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_item_2eproto);
    return ::descriptor_table_item_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kDataFieldNumber = 2,
    kItemTypeFieldNumber = 1,
  };
  // repeated .PBItemData data = 2;
  int data_size() const;
  private:
  int _internal_data_size() const;
  public:
  void clear_data();
  ::PBItemData* mutable_data(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PBItemData >*
      mutable_data();
  private:
  const ::PBItemData& _internal_data(int index) const;
  ::PBItemData* _internal_add_data();
  public:
  const ::PBItemData& data(int index) const;
  ::PBItemData* add_data();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PBItemData >&
      data() const;

  // .PBItemType item_type = 1;
  void clear_item_type();
  ::PBItemType item_type() const;
  void set_item_type(::PBItemType value);
  private:
  ::PBItemType _internal_item_type() const;
  void _internal_set_item_type(::PBItemType value);
  public:

  // @@protoc_insertion_point(class_scope:PBItemsDict)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PBItemData > data_;
  int item_type_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_item_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PBItemData

// string image_path = 1;
inline void PBItemData::clear_image_path() {
  image_path_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& PBItemData::image_path() const {
  // @@protoc_insertion_point(field_get:PBItemData.image_path)
  return _internal_image_path();
}
inline void PBItemData::set_image_path(const std::string& value) {
  _internal_set_image_path(value);
  // @@protoc_insertion_point(field_set:PBItemData.image_path)
}
inline std::string* PBItemData::mutable_image_path() {
  // @@protoc_insertion_point(field_mutable:PBItemData.image_path)
  return _internal_mutable_image_path();
}
inline const std::string& PBItemData::_internal_image_path() const {
  return image_path_.Get();
}
inline void PBItemData::_internal_set_image_path(const std::string& value) {
  
  image_path_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void PBItemData::set_image_path(std::string&& value) {
  
  image_path_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:PBItemData.image_path)
}
inline void PBItemData::set_image_path(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  image_path_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:PBItemData.image_path)
}
inline void PBItemData::set_image_path(const char* value,
    size_t size) {
  
  image_path_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:PBItemData.image_path)
}
inline std::string* PBItemData::_internal_mutable_image_path() {
  
  return image_path_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* PBItemData::release_image_path() {
  // @@protoc_insertion_point(field_release:PBItemData.image_path)
  return image_path_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void PBItemData::set_allocated_image_path(std::string* image_path) {
  if (image_path != nullptr) {
    
  } else {
    
  }
  image_path_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), image_path,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:PBItemData.image_path)
}
inline std::string* PBItemData::unsafe_arena_release_image_path() {
  // @@protoc_insertion_point(field_unsafe_arena_release:PBItemData.image_path)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return image_path_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void PBItemData::unsafe_arena_set_allocated_image_path(
    std::string* image_path) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (image_path != nullptr) {
    
  } else {
    
  }
  image_path_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      image_path, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:PBItemData.image_path)
}

// string item_name = 2;
inline void PBItemData::clear_item_name() {
  item_name_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& PBItemData::item_name() const {
  // @@protoc_insertion_point(field_get:PBItemData.item_name)
  return _internal_item_name();
}
inline void PBItemData::set_item_name(const std::string& value) {
  _internal_set_item_name(value);
  // @@protoc_insertion_point(field_set:PBItemData.item_name)
}
inline std::string* PBItemData::mutable_item_name() {
  // @@protoc_insertion_point(field_mutable:PBItemData.item_name)
  return _internal_mutable_item_name();
}
inline const std::string& PBItemData::_internal_item_name() const {
  return item_name_.Get();
}
inline void PBItemData::_internal_set_item_name(const std::string& value) {
  
  item_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void PBItemData::set_item_name(std::string&& value) {
  
  item_name_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:PBItemData.item_name)
}
inline void PBItemData::set_item_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  item_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:PBItemData.item_name)
}
inline void PBItemData::set_item_name(const char* value,
    size_t size) {
  
  item_name_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:PBItemData.item_name)
}
inline std::string* PBItemData::_internal_mutable_item_name() {
  
  return item_name_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* PBItemData::release_item_name() {
  // @@protoc_insertion_point(field_release:PBItemData.item_name)
  return item_name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void PBItemData::set_allocated_item_name(std::string* item_name) {
  if (item_name != nullptr) {
    
  } else {
    
  }
  item_name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), item_name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:PBItemData.item_name)
}
inline std::string* PBItemData::unsafe_arena_release_item_name() {
  // @@protoc_insertion_point(field_unsafe_arena_release:PBItemData.item_name)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return item_name_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void PBItemData::unsafe_arena_set_allocated_item_name(
    std::string* item_name) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (item_name != nullptr) {
    
  } else {
    
  }
  item_name_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      item_name, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:PBItemData.item_name)
}

// .PBItemEnum item_id = 3;
inline void PBItemData::clear_item_id() {
  item_id_ = 0;
}
inline ::PBItemEnum PBItemData::_internal_item_id() const {
  return static_cast< ::PBItemEnum >(item_id_);
}
inline ::PBItemEnum PBItemData::item_id() const {
  // @@protoc_insertion_point(field_get:PBItemData.item_id)
  return _internal_item_id();
}
inline void PBItemData::_internal_set_item_id(::PBItemEnum value) {
  
  item_id_ = value;
}
inline void PBItemData::set_item_id(::PBItemEnum value) {
  _internal_set_item_id(value);
  // @@protoc_insertion_point(field_set:PBItemData.item_id)
}

// int32 quantity = 4;
inline void PBItemData::clear_quantity() {
  quantity_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 PBItemData::_internal_quantity() const {
  return quantity_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 PBItemData::quantity() const {
  // @@protoc_insertion_point(field_get:PBItemData.quantity)
  return _internal_quantity();
}
inline void PBItemData::_internal_set_quantity(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  quantity_ = value;
}
inline void PBItemData::set_quantity(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_quantity(value);
  // @@protoc_insertion_point(field_set:PBItemData.quantity)
}

// -------------------------------------------------------------------

// PBItemsDict

// .PBItemType item_type = 1;
inline void PBItemsDict::clear_item_type() {
  item_type_ = 0;
}
inline ::PBItemType PBItemsDict::_internal_item_type() const {
  return static_cast< ::PBItemType >(item_type_);
}
inline ::PBItemType PBItemsDict::item_type() const {
  // @@protoc_insertion_point(field_get:PBItemsDict.item_type)
  return _internal_item_type();
}
inline void PBItemsDict::_internal_set_item_type(::PBItemType value) {
  
  item_type_ = value;
}
inline void PBItemsDict::set_item_type(::PBItemType value) {
  _internal_set_item_type(value);
  // @@protoc_insertion_point(field_set:PBItemsDict.item_type)
}

// repeated .PBItemData data = 2;
inline int PBItemsDict::_internal_data_size() const {
  return data_.size();
}
inline int PBItemsDict::data_size() const {
  return _internal_data_size();
}
inline void PBItemsDict::clear_data() {
  data_.Clear();
}
inline ::PBItemData* PBItemsDict::mutable_data(int index) {
  // @@protoc_insertion_point(field_mutable:PBItemsDict.data)
  return data_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PBItemData >*
PBItemsDict::mutable_data() {
  // @@protoc_insertion_point(field_mutable_list:PBItemsDict.data)
  return &data_;
}
inline const ::PBItemData& PBItemsDict::_internal_data(int index) const {
  return data_.Get(index);
}
inline const ::PBItemData& PBItemsDict::data(int index) const {
  // @@protoc_insertion_point(field_get:PBItemsDict.data)
  return _internal_data(index);
}
inline ::PBItemData* PBItemsDict::_internal_add_data() {
  return data_.Add();
}
inline ::PBItemData* PBItemsDict::add_data() {
  // @@protoc_insertion_point(field_add:PBItemsDict.data)
  return _internal_add_data();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::PBItemData >&
PBItemsDict::data() const {
  // @@protoc_insertion_point(field_list:PBItemsDict.data)
  return data_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::PBItemEnum> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PBItemEnum>() {
  return ::PBItemEnum_descriptor();
}
template <> struct is_proto_enum< ::PBItemType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PBItemType>() {
  return ::PBItemType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_item_2eproto
