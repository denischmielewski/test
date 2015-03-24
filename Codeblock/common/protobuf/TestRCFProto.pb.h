// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: TestRCFProto.proto

#ifndef PROTOBUF_TestRCFProto_2eproto__INCLUDED
#define PROTOBUF_TestRCFProto_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_TestRCFProto_2eproto();
void protobuf_AssignDesc_TestRCFProto_2eproto();
void protobuf_ShutdownFile_TestRCFProto_2eproto();

class PositionInformationTransmit;
class PositionInformationReceive;

// ===================================================================

class PositionInformationTransmit : public ::google::protobuf::Message {
 public:
  PositionInformationTransmit();
  virtual ~PositionInformationTransmit();

  PositionInformationTransmit(const PositionInformationTransmit& from);

  inline PositionInformationTransmit& operator=(const PositionInformationTransmit& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PositionInformationTransmit& default_instance();

  void Swap(PositionInformationTransmit* other);

  // implements Message ----------------------------------------------

  PositionInformationTransmit* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PositionInformationTransmit& from);
  void MergeFrom(const PositionInformationTransmit& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string trainID = 1;
  inline bool has_trainid() const;
  inline void clear_trainid();
  static const int kTrainIDFieldNumber = 1;
  inline const ::std::string& trainid() const;
  inline void set_trainid(const ::std::string& value);
  inline void set_trainid(const char* value);
  inline void set_trainid(const char* value, size_t size);
  inline ::std::string* mutable_trainid();
  inline ::std::string* release_trainid();
  inline void set_allocated_trainid(::std::string* trainid);

  // required string position = 2;
  inline bool has_position() const;
  inline void clear_position();
  static const int kPositionFieldNumber = 2;
  inline const ::std::string& position() const;
  inline void set_position(const ::std::string& value);
  inline void set_position(const char* value);
  inline void set_position(const char* value, size_t size);
  inline ::std::string* mutable_position();
  inline ::std::string* release_position();
  inline void set_allocated_position(::std::string* position);

  // optional int32 status = 3;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 3;
  inline ::google::protobuf::int32 status() const;
  inline void set_status(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:PositionInformationTransmit)
 private:
  inline void set_has_trainid();
  inline void clear_has_trainid();
  inline void set_has_position();
  inline void clear_has_position();
  inline void set_has_status();
  inline void clear_has_status();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* trainid_;
  ::std::string* position_;
  ::google::protobuf::int32 status_;
  friend void  protobuf_AddDesc_TestRCFProto_2eproto();
  friend void protobuf_AssignDesc_TestRCFProto_2eproto();
  friend void protobuf_ShutdownFile_TestRCFProto_2eproto();

  void InitAsDefaultInstance();
  static PositionInformationTransmit* default_instance_;
};
// -------------------------------------------------------------------

class PositionInformationReceive : public ::google::protobuf::Message {
 public:
  PositionInformationReceive();
  virtual ~PositionInformationReceive();

  PositionInformationReceive(const PositionInformationReceive& from);

  inline PositionInformationReceive& operator=(const PositionInformationReceive& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const PositionInformationReceive& default_instance();

  void Swap(PositionInformationReceive* other);

  // implements Message ----------------------------------------------

  PositionInformationReceive* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PositionInformationReceive& from);
  void MergeFrom(const PositionInformationReceive& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string serverName = 2;
  inline bool has_servername() const;
  inline void clear_servername();
  static const int kServerNameFieldNumber = 2;
  inline const ::std::string& servername() const;
  inline void set_servername(const ::std::string& value);
  inline void set_servername(const char* value);
  inline void set_servername(const char* value, size_t size);
  inline ::std::string* mutable_servername();
  inline ::std::string* release_servername();
  inline void set_allocated_servername(::std::string* servername);

  // @@protoc_insertion_point(class_scope:PositionInformationReceive)
 private:
  inline void set_has_servername();
  inline void clear_has_servername();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* servername_;
  friend void  protobuf_AddDesc_TestRCFProto_2eproto();
  friend void protobuf_AssignDesc_TestRCFProto_2eproto();
  friend void protobuf_ShutdownFile_TestRCFProto_2eproto();

  void InitAsDefaultInstance();
  static PositionInformationReceive* default_instance_;
};
// ===================================================================

class PositionInformationService_Stub;

class PositionInformationService : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline PositionInformationService() {};
 public:
  virtual ~PositionInformationService();

  typedef PositionInformationService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void PositionInformation(::google::protobuf::RpcController* controller,
                       const ::PositionInformationTransmit* request,
                       ::PositionInformationReceive* response,
                       ::google::protobuf::Closure* done);

  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(PositionInformationService);
};

class PositionInformationService_Stub : public PositionInformationService {
 public:
  PositionInformationService_Stub(::google::protobuf::RpcChannel* channel);
  PositionInformationService_Stub(::google::protobuf::RpcChannel* channel,
                   ::google::protobuf::Service::ChannelOwnership ownership);
  ~PositionInformationService_Stub();

  inline ::google::protobuf::RpcChannel* channel() { return channel_; }

  // implements PositionInformationService ------------------------------------------

  void PositionInformation(::google::protobuf::RpcController* controller,
                       const ::PositionInformationTransmit* request,
                       ::PositionInformationReceive* response,
                       ::google::protobuf::Closure* done);
 private:
  ::google::protobuf::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(PositionInformationService_Stub);
};


// ===================================================================


// ===================================================================

// PositionInformationTransmit

// required string trainID = 1;
inline bool PositionInformationTransmit::has_trainid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PositionInformationTransmit::set_has_trainid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PositionInformationTransmit::clear_has_trainid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PositionInformationTransmit::clear_trainid() {
  if (trainid_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    trainid_->clear();
  }
  clear_has_trainid();
}
inline const ::std::string& PositionInformationTransmit::trainid() const {
  // @@protoc_insertion_point(field_get:PositionInformationTransmit.trainID)
  return *trainid_;
}
inline void PositionInformationTransmit::set_trainid(const ::std::string& value) {
  set_has_trainid();
  if (trainid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    trainid_ = new ::std::string;
  }
  trainid_->assign(value);
  // @@protoc_insertion_point(field_set:PositionInformationTransmit.trainID)
}
inline void PositionInformationTransmit::set_trainid(const char* value) {
  set_has_trainid();
  if (trainid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    trainid_ = new ::std::string;
  }
  trainid_->assign(value);
  // @@protoc_insertion_point(field_set_char:PositionInformationTransmit.trainID)
}
inline void PositionInformationTransmit::set_trainid(const char* value, size_t size) {
  set_has_trainid();
  if (trainid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    trainid_ = new ::std::string;
  }
  trainid_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:PositionInformationTransmit.trainID)
}
inline ::std::string* PositionInformationTransmit::mutable_trainid() {
  set_has_trainid();
  if (trainid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    trainid_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:PositionInformationTransmit.trainID)
  return trainid_;
}
inline ::std::string* PositionInformationTransmit::release_trainid() {
  clear_has_trainid();
  if (trainid_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = trainid_;
    trainid_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void PositionInformationTransmit::set_allocated_trainid(::std::string* trainid) {
  if (trainid_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete trainid_;
  }
  if (trainid) {
    set_has_trainid();
    trainid_ = trainid;
  } else {
    clear_has_trainid();
    trainid_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:PositionInformationTransmit.trainID)
}

// required string position = 2;
inline bool PositionInformationTransmit::has_position() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PositionInformationTransmit::set_has_position() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PositionInformationTransmit::clear_has_position() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PositionInformationTransmit::clear_position() {
  if (position_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    position_->clear();
  }
  clear_has_position();
}
inline const ::std::string& PositionInformationTransmit::position() const {
  // @@protoc_insertion_point(field_get:PositionInformationTransmit.position)
  return *position_;
}
inline void PositionInformationTransmit::set_position(const ::std::string& value) {
  set_has_position();
  if (position_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    position_ = new ::std::string;
  }
  position_->assign(value);
  // @@protoc_insertion_point(field_set:PositionInformationTransmit.position)
}
inline void PositionInformationTransmit::set_position(const char* value) {
  set_has_position();
  if (position_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    position_ = new ::std::string;
  }
  position_->assign(value);
  // @@protoc_insertion_point(field_set_char:PositionInformationTransmit.position)
}
inline void PositionInformationTransmit::set_position(const char* value, size_t size) {
  set_has_position();
  if (position_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    position_ = new ::std::string;
  }
  position_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:PositionInformationTransmit.position)
}
inline ::std::string* PositionInformationTransmit::mutable_position() {
  set_has_position();
  if (position_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    position_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:PositionInformationTransmit.position)
  return position_;
}
inline ::std::string* PositionInformationTransmit::release_position() {
  clear_has_position();
  if (position_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = position_;
    position_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void PositionInformationTransmit::set_allocated_position(::std::string* position) {
  if (position_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete position_;
  }
  if (position) {
    set_has_position();
    position_ = position;
  } else {
    clear_has_position();
    position_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:PositionInformationTransmit.position)
}

// optional int32 status = 3;
inline bool PositionInformationTransmit::has_status() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PositionInformationTransmit::set_has_status() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PositionInformationTransmit::clear_has_status() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PositionInformationTransmit::clear_status() {
  status_ = 0;
  clear_has_status();
}
inline ::google::protobuf::int32 PositionInformationTransmit::status() const {
  // @@protoc_insertion_point(field_get:PositionInformationTransmit.status)
  return status_;
}
inline void PositionInformationTransmit::set_status(::google::protobuf::int32 value) {
  set_has_status();
  status_ = value;
  // @@protoc_insertion_point(field_set:PositionInformationTransmit.status)
}

// -------------------------------------------------------------------

// PositionInformationReceive

// required string serverName = 2;
inline bool PositionInformationReceive::has_servername() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PositionInformationReceive::set_has_servername() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PositionInformationReceive::clear_has_servername() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PositionInformationReceive::clear_servername() {
  if (servername_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    servername_->clear();
  }
  clear_has_servername();
}
inline const ::std::string& PositionInformationReceive::servername() const {
  // @@protoc_insertion_point(field_get:PositionInformationReceive.serverName)
  return *servername_;
}
inline void PositionInformationReceive::set_servername(const ::std::string& value) {
  set_has_servername();
  if (servername_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    servername_ = new ::std::string;
  }
  servername_->assign(value);
  // @@protoc_insertion_point(field_set:PositionInformationReceive.serverName)
}
inline void PositionInformationReceive::set_servername(const char* value) {
  set_has_servername();
  if (servername_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    servername_ = new ::std::string;
  }
  servername_->assign(value);
  // @@protoc_insertion_point(field_set_char:PositionInformationReceive.serverName)
}
inline void PositionInformationReceive::set_servername(const char* value, size_t size) {
  set_has_servername();
  if (servername_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    servername_ = new ::std::string;
  }
  servername_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:PositionInformationReceive.serverName)
}
inline ::std::string* PositionInformationReceive::mutable_servername() {
  set_has_servername();
  if (servername_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    servername_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:PositionInformationReceive.serverName)
  return servername_;
}
inline ::std::string* PositionInformationReceive::release_servername() {
  clear_has_servername();
  if (servername_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = servername_;
    servername_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void PositionInformationReceive::set_allocated_servername(::std::string* servername) {
  if (servername_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete servername_;
  }
  if (servername) {
    set_has_servername();
    servername_ = servername;
  } else {
    clear_has_servername();
    servername_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:PositionInformationReceive.serverName)
}


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_TestRCFProto_2eproto__INCLUDED
