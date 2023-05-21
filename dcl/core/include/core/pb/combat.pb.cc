// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: combat.proto

#include "combat.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_player_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<3> scc_info_PBPlayer_player_2eproto;
class PBCombatDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<PBCombat> _instance;
} _PBCombat_default_instance_;
static void InitDefaultsscc_info_PBCombat_combat_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::_PBCombat_default_instance_;
    new (ptr) ::PBCombat();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::PBCombat::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_PBCombat_combat_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_PBCombat_combat_2eproto}, {
      &scc_info_PBPlayer_player_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_combat_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_combat_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_combat_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_combat_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::PBCombat, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::PBCombat, starter_user_id_),
  PROTOBUF_FIELD_OFFSET(::PBCombat, opponent_user_id_),
  PROTOBUF_FIELD_OFFSET(::PBCombat, wager_),
  PROTOBUF_FIELD_OFFSET(::PBCombat, turn_),
  PROTOBUF_FIELD_OFFSET(::PBCombat, starter_user_info_),
  PROTOBUF_FIELD_OFFSET(::PBCombat, opponent_user_info_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::PBCombat)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_PBCombat_default_instance_),
};

const char descriptor_table_protodef_combat_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014combat.proto\032\014player.proto\"\247\001\n\010PBComba"
  "t\022\027\n\017starter_user_id\030\001 \001(\003\022\030\n\020opponent_u"
  "ser_id\030\002 \001(\003\022\r\n\005wager\030\003 \001(\005\022\014\n\004turn\030\004 \001("
  "\005\022$\n\021starter_user_info\030\005 \001(\0132\t.PBPlayer\022"
  "%\n\022opponent_user_info\030\006 \001(\0132\t.PBPlayer*B"
  "\n\017PBCombatActions\022\n\n\006ATTACK\020\000\022\t\n\005DODGE\020\001"
  "\022\t\n\005BLOCK\020\002\022\r\n\tSURRENDER\020\003b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_combat_2eproto_deps[1] = {
  &::descriptor_table_player_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_combat_2eproto_sccs[1] = {
  &scc_info_PBCombat_combat_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_combat_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_combat_2eproto = {
  false, false, descriptor_table_protodef_combat_2eproto, "combat.proto", 274,
  &descriptor_table_combat_2eproto_once, descriptor_table_combat_2eproto_sccs, descriptor_table_combat_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_combat_2eproto::offsets,
  file_level_metadata_combat_2eproto, 1, file_level_enum_descriptors_combat_2eproto, file_level_service_descriptors_combat_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_combat_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_combat_2eproto)), true);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PBCombatActions_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_combat_2eproto);
  return file_level_enum_descriptors_combat_2eproto[0];
}
bool PBCombatActions_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}


// ===================================================================

void PBCombat::InitAsDefaultInstance() {
  ::_PBCombat_default_instance_._instance.get_mutable()->starter_user_info_ = const_cast< ::PBPlayer*>(
      ::PBPlayer::internal_default_instance());
  ::_PBCombat_default_instance_._instance.get_mutable()->opponent_user_info_ = const_cast< ::PBPlayer*>(
      ::PBPlayer::internal_default_instance());
}
class PBCombat::_Internal {
 public:
  static const ::PBPlayer& starter_user_info(const PBCombat* msg);
  static const ::PBPlayer& opponent_user_info(const PBCombat* msg);
};

const ::PBPlayer&
PBCombat::_Internal::starter_user_info(const PBCombat* msg) {
  return *msg->starter_user_info_;
}
const ::PBPlayer&
PBCombat::_Internal::opponent_user_info(const PBCombat* msg) {
  return *msg->opponent_user_info_;
}
void PBCombat::clear_starter_user_info() {
  if (GetArena() == nullptr && starter_user_info_ != nullptr) {
    delete starter_user_info_;
  }
  starter_user_info_ = nullptr;
}
void PBCombat::clear_opponent_user_info() {
  if (GetArena() == nullptr && opponent_user_info_ != nullptr) {
    delete opponent_user_info_;
  }
  opponent_user_info_ = nullptr;
}
PBCombat::PBCombat(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:PBCombat)
}
PBCombat::PBCombat(const PBCombat& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_starter_user_info()) {
    starter_user_info_ = new ::PBPlayer(*from.starter_user_info_);
  } else {
    starter_user_info_ = nullptr;
  }
  if (from._internal_has_opponent_user_info()) {
    opponent_user_info_ = new ::PBPlayer(*from.opponent_user_info_);
  } else {
    opponent_user_info_ = nullptr;
  }
  ::memcpy(&starter_user_id_, &from.starter_user_id_,
    static_cast<size_t>(reinterpret_cast<char*>(&turn_) -
    reinterpret_cast<char*>(&starter_user_id_)) + sizeof(turn_));
  // @@protoc_insertion_point(copy_constructor:PBCombat)
}

void PBCombat::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_PBCombat_combat_2eproto.base);
  ::memset(&starter_user_info_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&turn_) -
      reinterpret_cast<char*>(&starter_user_info_)) + sizeof(turn_));
}

PBCombat::~PBCombat() {
  // @@protoc_insertion_point(destructor:PBCombat)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void PBCombat::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  if (this != internal_default_instance()) delete starter_user_info_;
  if (this != internal_default_instance()) delete opponent_user_info_;
}

void PBCombat::ArenaDtor(void* object) {
  PBCombat* _this = reinterpret_cast< PBCombat* >(object);
  (void)_this;
}
void PBCombat::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void PBCombat::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const PBCombat& PBCombat::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_PBCombat_combat_2eproto.base);
  return *internal_default_instance();
}


void PBCombat::Clear() {
// @@protoc_insertion_point(message_clear_start:PBCombat)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArena() == nullptr && starter_user_info_ != nullptr) {
    delete starter_user_info_;
  }
  starter_user_info_ = nullptr;
  if (GetArena() == nullptr && opponent_user_info_ != nullptr) {
    delete opponent_user_info_;
  }
  opponent_user_info_ = nullptr;
  ::memset(&starter_user_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&turn_) -
      reinterpret_cast<char*>(&starter_user_id_)) + sizeof(turn_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* PBCombat::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int64 starter_user_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          starter_user_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int64 opponent_user_id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          opponent_user_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 wager = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          wager_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 turn = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          turn_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .PBPlayer starter_user_info = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 42)) {
          ptr = ctx->ParseMessage(_internal_mutable_starter_user_info(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .PBPlayer opponent_user_info = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 50)) {
          ptr = ctx->ParseMessage(_internal_mutable_opponent_user_info(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* PBCombat::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:PBCombat)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 starter_user_id = 1;
  if (this->starter_user_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(1, this->_internal_starter_user_id(), target);
  }

  // int64 opponent_user_id = 2;
  if (this->opponent_user_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(2, this->_internal_opponent_user_id(), target);
  }

  // int32 wager = 3;
  if (this->wager() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_wager(), target);
  }

  // int32 turn = 4;
  if (this->turn() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(4, this->_internal_turn(), target);
  }

  // .PBPlayer starter_user_info = 5;
  if (this->has_starter_user_info()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        5, _Internal::starter_user_info(this), target, stream);
  }

  // .PBPlayer opponent_user_info = 6;
  if (this->has_opponent_user_info()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        6, _Internal::opponent_user_info(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PBCombat)
  return target;
}

size_t PBCombat::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:PBCombat)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .PBPlayer starter_user_info = 5;
  if (this->has_starter_user_info()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *starter_user_info_);
  }

  // .PBPlayer opponent_user_info = 6;
  if (this->has_opponent_user_info()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *opponent_user_info_);
  }

  // int64 starter_user_id = 1;
  if (this->starter_user_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_starter_user_id());
  }

  // int64 opponent_user_id = 2;
  if (this->opponent_user_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_opponent_user_id());
  }

  // int32 wager = 3;
  if (this->wager() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_wager());
  }

  // int32 turn = 4;
  if (this->turn() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_turn());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void PBCombat::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:PBCombat)
  GOOGLE_DCHECK_NE(&from, this);
  const PBCombat* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<PBCombat>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:PBCombat)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:PBCombat)
    MergeFrom(*source);
  }
}

void PBCombat::MergeFrom(const PBCombat& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:PBCombat)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_starter_user_info()) {
    _internal_mutable_starter_user_info()->::PBPlayer::MergeFrom(from._internal_starter_user_info());
  }
  if (from.has_opponent_user_info()) {
    _internal_mutable_opponent_user_info()->::PBPlayer::MergeFrom(from._internal_opponent_user_info());
  }
  if (from.starter_user_id() != 0) {
    _internal_set_starter_user_id(from._internal_starter_user_id());
  }
  if (from.opponent_user_id() != 0) {
    _internal_set_opponent_user_id(from._internal_opponent_user_id());
  }
  if (from.wager() != 0) {
    _internal_set_wager(from._internal_wager());
  }
  if (from.turn() != 0) {
    _internal_set_turn(from._internal_turn());
  }
}

void PBCombat::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:PBCombat)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PBCombat::CopyFrom(const PBCombat& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:PBCombat)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PBCombat::IsInitialized() const {
  return true;
}

void PBCombat::InternalSwap(PBCombat* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(PBCombat, turn_)
      + sizeof(PBCombat::turn_)
      - PROTOBUF_FIELD_OFFSET(PBCombat, starter_user_info_)>(
          reinterpret_cast<char*>(&starter_user_info_),
          reinterpret_cast<char*>(&other->starter_user_info_));
}

::PROTOBUF_NAMESPACE_ID::Metadata PBCombat::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::PBCombat* Arena::CreateMaybeMessage< ::PBCombat >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PBCombat >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
