// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: combat.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_combat_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_combat_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "player.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_combat_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_combat_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_combat_2eproto;
class PBCombat;
struct PBCombatDefaultTypeInternal;
extern PBCombatDefaultTypeInternal _PBCombat_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::PBCombat* Arena::CreateMaybeMessage<::PBCombat>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

enum PBCombatActions : int {
  CA_NONE = 0,
  CA_ATTACK = 1,
  CA_DODGE = 2,
  CA_BLOCK = 3,
  CA_SURRENDER = 4,
  PBCombatActions_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  PBCombatActions_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool PBCombatActions_IsValid(int value);
constexpr PBCombatActions PBCombatActions_MIN = CA_NONE;
constexpr PBCombatActions PBCombatActions_MAX = CA_SURRENDER;
constexpr int PBCombatActions_ARRAYSIZE = PBCombatActions_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PBCombatActions_descriptor();
template<typename T>
inline const std::string& PBCombatActions_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PBCombatActions>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PBCombatActions_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PBCombatActions_descriptor(), enum_t_value);
}
inline bool PBCombatActions_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, PBCombatActions* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PBCombatActions>(
    PBCombatActions_descriptor(), name, value);
}
// ===================================================================

class PBCombat final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:PBCombat) */ {
 public:
  inline PBCombat() : PBCombat(nullptr) {}
  ~PBCombat() override;
  explicit PROTOBUF_CONSTEXPR PBCombat(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  PBCombat(const PBCombat& from);
  PBCombat(PBCombat&& from) noexcept
    : PBCombat() {
    *this = ::std::move(from);
  }

  inline PBCombat& operator=(const PBCombat& from) {
    CopyFrom(from);
    return *this;
  }
  inline PBCombat& operator=(PBCombat&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const PBCombat& default_instance() {
    return *internal_default_instance();
  }
  static inline const PBCombat* internal_default_instance() {
    return reinterpret_cast<const PBCombat*>(
               &_PBCombat_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(PBCombat& a, PBCombat& b) {
    a.Swap(&b);
  }
  inline void Swap(PBCombat* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PBCombat* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  PBCombat* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<PBCombat>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const PBCombat& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const PBCombat& from) {
    PBCombat::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(PBCombat* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "PBCombat";
  }
  protected:
  explicit PBCombat(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStarterUserInfoFieldNumber = 9,
    kOpponentUserInfoFieldNumber = 10,
    kStarterUserIdFieldNumber = 1,
    kOpponentUserIdFieldNumber = 2,
    kWagerFieldNumber = 3,
    kTurnFieldNumber = 4,
    kStarterActionFieldNumber = 5,
    kOpponentActionFieldNumber = 6,
    kStarterPlayerIdFieldNumber = 7,
    kOpponentPlayerIdFieldNumber = 8,
  };
  // .PBPlayer starter_user_info = 9;
  bool has_starter_user_info() const;
  private:
  bool _internal_has_starter_user_info() const;
  public:
  void clear_starter_user_info();
  const ::PBPlayer& starter_user_info() const;
  PROTOBUF_NODISCARD ::PBPlayer* release_starter_user_info();
  ::PBPlayer* mutable_starter_user_info();
  void set_allocated_starter_user_info(::PBPlayer* starter_user_info);
  private:
  const ::PBPlayer& _internal_starter_user_info() const;
  ::PBPlayer* _internal_mutable_starter_user_info();
  public:
  void unsafe_arena_set_allocated_starter_user_info(
      ::PBPlayer* starter_user_info);
  ::PBPlayer* unsafe_arena_release_starter_user_info();

  // .PBPlayer opponent_user_info = 10;
  bool has_opponent_user_info() const;
  private:
  bool _internal_has_opponent_user_info() const;
  public:
  void clear_opponent_user_info();
  const ::PBPlayer& opponent_user_info() const;
  PROTOBUF_NODISCARD ::PBPlayer* release_opponent_user_info();
  ::PBPlayer* mutable_opponent_user_info();
  void set_allocated_opponent_user_info(::PBPlayer* opponent_user_info);
  private:
  const ::PBPlayer& _internal_opponent_user_info() const;
  ::PBPlayer* _internal_mutable_opponent_user_info();
  public:
  void unsafe_arena_set_allocated_opponent_user_info(
      ::PBPlayer* opponent_user_info);
  ::PBPlayer* unsafe_arena_release_opponent_user_info();

  // int64 starter_user_id = 1;
  void clear_starter_user_id();
  int64_t starter_user_id() const;
  void set_starter_user_id(int64_t value);
  private:
  int64_t _internal_starter_user_id() const;
  void _internal_set_starter_user_id(int64_t value);
  public:

  // int64 opponent_user_id = 2;
  void clear_opponent_user_id();
  int64_t opponent_user_id() const;
  void set_opponent_user_id(int64_t value);
  private:
  int64_t _internal_opponent_user_id() const;
  void _internal_set_opponent_user_id(int64_t value);
  public:

  // int32 wager = 3;
  void clear_wager();
  int32_t wager() const;
  void set_wager(int32_t value);
  private:
  int32_t _internal_wager() const;
  void _internal_set_wager(int32_t value);
  public:

  // int32 turn = 4;
  void clear_turn();
  int32_t turn() const;
  void set_turn(int32_t value);
  private:
  int32_t _internal_turn() const;
  void _internal_set_turn(int32_t value);
  public:

  // .PBCombatActions starter_action = 5;
  void clear_starter_action();
  ::PBCombatActions starter_action() const;
  void set_starter_action(::PBCombatActions value);
  private:
  ::PBCombatActions _internal_starter_action() const;
  void _internal_set_starter_action(::PBCombatActions value);
  public:

  // .PBCombatActions opponent_action = 6;
  void clear_opponent_action();
  ::PBCombatActions opponent_action() const;
  void set_opponent_action(::PBCombatActions value);
  private:
  ::PBCombatActions _internal_opponent_action() const;
  void _internal_set_opponent_action(::PBCombatActions value);
  public:

  // int32 starter_player_id = 7;
  void clear_starter_player_id();
  int32_t starter_player_id() const;
  void set_starter_player_id(int32_t value);
  private:
  int32_t _internal_starter_player_id() const;
  void _internal_set_starter_player_id(int32_t value);
  public:

  // int32 opponent_player_id = 8;
  void clear_opponent_player_id();
  int32_t opponent_player_id() const;
  void set_opponent_player_id(int32_t value);
  private:
  int32_t _internal_opponent_player_id() const;
  void _internal_set_opponent_player_id(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:PBCombat)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PBPlayer* starter_user_info_;
    ::PBPlayer* opponent_user_info_;
    int64_t starter_user_id_;
    int64_t opponent_user_id_;
    int32_t wager_;
    int32_t turn_;
    int starter_action_;
    int opponent_action_;
    int32_t starter_player_id_;
    int32_t opponent_player_id_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_combat_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PBCombat

// int64 starter_user_id = 1;
inline void PBCombat::clear_starter_user_id() {
  _impl_.starter_user_id_ = int64_t{0};
}
inline int64_t PBCombat::_internal_starter_user_id() const {
  return _impl_.starter_user_id_;
}
inline int64_t PBCombat::starter_user_id() const {
  // @@protoc_insertion_point(field_get:PBCombat.starter_user_id)
  return _internal_starter_user_id();
}
inline void PBCombat::_internal_set_starter_user_id(int64_t value) {
  
  _impl_.starter_user_id_ = value;
}
inline void PBCombat::set_starter_user_id(int64_t value) {
  _internal_set_starter_user_id(value);
  // @@protoc_insertion_point(field_set:PBCombat.starter_user_id)
}

// int64 opponent_user_id = 2;
inline void PBCombat::clear_opponent_user_id() {
  _impl_.opponent_user_id_ = int64_t{0};
}
inline int64_t PBCombat::_internal_opponent_user_id() const {
  return _impl_.opponent_user_id_;
}
inline int64_t PBCombat::opponent_user_id() const {
  // @@protoc_insertion_point(field_get:PBCombat.opponent_user_id)
  return _internal_opponent_user_id();
}
inline void PBCombat::_internal_set_opponent_user_id(int64_t value) {
  
  _impl_.opponent_user_id_ = value;
}
inline void PBCombat::set_opponent_user_id(int64_t value) {
  _internal_set_opponent_user_id(value);
  // @@protoc_insertion_point(field_set:PBCombat.opponent_user_id)
}

// int32 wager = 3;
inline void PBCombat::clear_wager() {
  _impl_.wager_ = 0;
}
inline int32_t PBCombat::_internal_wager() const {
  return _impl_.wager_;
}
inline int32_t PBCombat::wager() const {
  // @@protoc_insertion_point(field_get:PBCombat.wager)
  return _internal_wager();
}
inline void PBCombat::_internal_set_wager(int32_t value) {
  
  _impl_.wager_ = value;
}
inline void PBCombat::set_wager(int32_t value) {
  _internal_set_wager(value);
  // @@protoc_insertion_point(field_set:PBCombat.wager)
}

// int32 turn = 4;
inline void PBCombat::clear_turn() {
  _impl_.turn_ = 0;
}
inline int32_t PBCombat::_internal_turn() const {
  return _impl_.turn_;
}
inline int32_t PBCombat::turn() const {
  // @@protoc_insertion_point(field_get:PBCombat.turn)
  return _internal_turn();
}
inline void PBCombat::_internal_set_turn(int32_t value) {
  
  _impl_.turn_ = value;
}
inline void PBCombat::set_turn(int32_t value) {
  _internal_set_turn(value);
  // @@protoc_insertion_point(field_set:PBCombat.turn)
}

// .PBCombatActions starter_action = 5;
inline void PBCombat::clear_starter_action() {
  _impl_.starter_action_ = 0;
}
inline ::PBCombatActions PBCombat::_internal_starter_action() const {
  return static_cast< ::PBCombatActions >(_impl_.starter_action_);
}
inline ::PBCombatActions PBCombat::starter_action() const {
  // @@protoc_insertion_point(field_get:PBCombat.starter_action)
  return _internal_starter_action();
}
inline void PBCombat::_internal_set_starter_action(::PBCombatActions value) {
  
  _impl_.starter_action_ = value;
}
inline void PBCombat::set_starter_action(::PBCombatActions value) {
  _internal_set_starter_action(value);
  // @@protoc_insertion_point(field_set:PBCombat.starter_action)
}

// .PBCombatActions opponent_action = 6;
inline void PBCombat::clear_opponent_action() {
  _impl_.opponent_action_ = 0;
}
inline ::PBCombatActions PBCombat::_internal_opponent_action() const {
  return static_cast< ::PBCombatActions >(_impl_.opponent_action_);
}
inline ::PBCombatActions PBCombat::opponent_action() const {
  // @@protoc_insertion_point(field_get:PBCombat.opponent_action)
  return _internal_opponent_action();
}
inline void PBCombat::_internal_set_opponent_action(::PBCombatActions value) {
  
  _impl_.opponent_action_ = value;
}
inline void PBCombat::set_opponent_action(::PBCombatActions value) {
  _internal_set_opponent_action(value);
  // @@protoc_insertion_point(field_set:PBCombat.opponent_action)
}

// int32 starter_player_id = 7;
inline void PBCombat::clear_starter_player_id() {
  _impl_.starter_player_id_ = 0;
}
inline int32_t PBCombat::_internal_starter_player_id() const {
  return _impl_.starter_player_id_;
}
inline int32_t PBCombat::starter_player_id() const {
  // @@protoc_insertion_point(field_get:PBCombat.starter_player_id)
  return _internal_starter_player_id();
}
inline void PBCombat::_internal_set_starter_player_id(int32_t value) {
  
  _impl_.starter_player_id_ = value;
}
inline void PBCombat::set_starter_player_id(int32_t value) {
  _internal_set_starter_player_id(value);
  // @@protoc_insertion_point(field_set:PBCombat.starter_player_id)
}

// int32 opponent_player_id = 8;
inline void PBCombat::clear_opponent_player_id() {
  _impl_.opponent_player_id_ = 0;
}
inline int32_t PBCombat::_internal_opponent_player_id() const {
  return _impl_.opponent_player_id_;
}
inline int32_t PBCombat::opponent_player_id() const {
  // @@protoc_insertion_point(field_get:PBCombat.opponent_player_id)
  return _internal_opponent_player_id();
}
inline void PBCombat::_internal_set_opponent_player_id(int32_t value) {
  
  _impl_.opponent_player_id_ = value;
}
inline void PBCombat::set_opponent_player_id(int32_t value) {
  _internal_set_opponent_player_id(value);
  // @@protoc_insertion_point(field_set:PBCombat.opponent_player_id)
}

// .PBPlayer starter_user_info = 9;
inline bool PBCombat::_internal_has_starter_user_info() const {
  return this != internal_default_instance() && _impl_.starter_user_info_ != nullptr;
}
inline bool PBCombat::has_starter_user_info() const {
  return _internal_has_starter_user_info();
}
inline const ::PBPlayer& PBCombat::_internal_starter_user_info() const {
  const ::PBPlayer* p = _impl_.starter_user_info_;
  return p != nullptr ? *p : reinterpret_cast<const ::PBPlayer&>(
      ::_PBPlayer_default_instance_);
}
inline const ::PBPlayer& PBCombat::starter_user_info() const {
  // @@protoc_insertion_point(field_get:PBCombat.starter_user_info)
  return _internal_starter_user_info();
}
inline void PBCombat::unsafe_arena_set_allocated_starter_user_info(
    ::PBPlayer* starter_user_info) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.starter_user_info_);
  }
  _impl_.starter_user_info_ = starter_user_info;
  if (starter_user_info) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:PBCombat.starter_user_info)
}
inline ::PBPlayer* PBCombat::release_starter_user_info() {
  
  ::PBPlayer* temp = _impl_.starter_user_info_;
  _impl_.starter_user_info_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::PBPlayer* PBCombat::unsafe_arena_release_starter_user_info() {
  // @@protoc_insertion_point(field_release:PBCombat.starter_user_info)
  
  ::PBPlayer* temp = _impl_.starter_user_info_;
  _impl_.starter_user_info_ = nullptr;
  return temp;
}
inline ::PBPlayer* PBCombat::_internal_mutable_starter_user_info() {
  
  if (_impl_.starter_user_info_ == nullptr) {
    auto* p = CreateMaybeMessage<::PBPlayer>(GetArenaForAllocation());
    _impl_.starter_user_info_ = p;
  }
  return _impl_.starter_user_info_;
}
inline ::PBPlayer* PBCombat::mutable_starter_user_info() {
  ::PBPlayer* _msg = _internal_mutable_starter_user_info();
  // @@protoc_insertion_point(field_mutable:PBCombat.starter_user_info)
  return _msg;
}
inline void PBCombat::set_allocated_starter_user_info(::PBPlayer* starter_user_info) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.starter_user_info_);
  }
  if (starter_user_info) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(starter_user_info));
    if (message_arena != submessage_arena) {
      starter_user_info = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, starter_user_info, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.starter_user_info_ = starter_user_info;
  // @@protoc_insertion_point(field_set_allocated:PBCombat.starter_user_info)
}

// .PBPlayer opponent_user_info = 10;
inline bool PBCombat::_internal_has_opponent_user_info() const {
  return this != internal_default_instance() && _impl_.opponent_user_info_ != nullptr;
}
inline bool PBCombat::has_opponent_user_info() const {
  return _internal_has_opponent_user_info();
}
inline const ::PBPlayer& PBCombat::_internal_opponent_user_info() const {
  const ::PBPlayer* p = _impl_.opponent_user_info_;
  return p != nullptr ? *p : reinterpret_cast<const ::PBPlayer&>(
      ::_PBPlayer_default_instance_);
}
inline const ::PBPlayer& PBCombat::opponent_user_info() const {
  // @@protoc_insertion_point(field_get:PBCombat.opponent_user_info)
  return _internal_opponent_user_info();
}
inline void PBCombat::unsafe_arena_set_allocated_opponent_user_info(
    ::PBPlayer* opponent_user_info) {
  if (GetArenaForAllocation() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.opponent_user_info_);
  }
  _impl_.opponent_user_info_ = opponent_user_info;
  if (opponent_user_info) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:PBCombat.opponent_user_info)
}
inline ::PBPlayer* PBCombat::release_opponent_user_info() {
  
  ::PBPlayer* temp = _impl_.opponent_user_info_;
  _impl_.opponent_user_info_ = nullptr;
#ifdef PROTOBUF_FORCE_COPY_IN_RELEASE
  auto* old =  reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(temp);
  temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  if (GetArenaForAllocation() == nullptr) { delete old; }
#else  // PROTOBUF_FORCE_COPY_IN_RELEASE
  if (GetArenaForAllocation() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
#endif  // !PROTOBUF_FORCE_COPY_IN_RELEASE
  return temp;
}
inline ::PBPlayer* PBCombat::unsafe_arena_release_opponent_user_info() {
  // @@protoc_insertion_point(field_release:PBCombat.opponent_user_info)
  
  ::PBPlayer* temp = _impl_.opponent_user_info_;
  _impl_.opponent_user_info_ = nullptr;
  return temp;
}
inline ::PBPlayer* PBCombat::_internal_mutable_opponent_user_info() {
  
  if (_impl_.opponent_user_info_ == nullptr) {
    auto* p = CreateMaybeMessage<::PBPlayer>(GetArenaForAllocation());
    _impl_.opponent_user_info_ = p;
  }
  return _impl_.opponent_user_info_;
}
inline ::PBPlayer* PBCombat::mutable_opponent_user_info() {
  ::PBPlayer* _msg = _internal_mutable_opponent_user_info();
  // @@protoc_insertion_point(field_mutable:PBCombat.opponent_user_info)
  return _msg;
}
inline void PBCombat::set_allocated_opponent_user_info(::PBPlayer* opponent_user_info) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  if (message_arena == nullptr) {
    delete reinterpret_cast< ::PROTOBUF_NAMESPACE_ID::MessageLite*>(_impl_.opponent_user_info_);
  }
  if (opponent_user_info) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
        ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(
                reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(opponent_user_info));
    if (message_arena != submessage_arena) {
      opponent_user_info = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, opponent_user_info, submessage_arena);
    }
    
  } else {
    
  }
  _impl_.opponent_user_info_ = opponent_user_info;
  // @@protoc_insertion_point(field_set_allocated:PBCombat.opponent_user_info)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::PBCombatActions> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::PBCombatActions>() {
  return ::PBCombatActions_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_combat_2eproto
