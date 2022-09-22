#include "RioFileDescriptor.hpp"

RioFileDescriptor::RioFileDescriptor(const Fd &fd)
    : fd_(fd.GetFd()), buffer_remaining_(0) {
  buf_pos_ptr_ = buf_;
}
ssize_t RioFileDescriptor::WriteNbyte(const char *usr_buf, size_t n) {
  size_t byte = n;
  ssize_t written_size = 0;
  const char *buf_ptr = usr_buf;
  while (byte > 0) {
    written_size = write(fd_.GetFd(), buf_ptr, byte);
    // written_size = write(1, buf_ptr, byte);
    byte -= written_size;
    buf_ptr += written_size;
  }
  return n;
}

ssize_t RioFileDescriptor::ReadLineByteEach(char *usr_buf, size_t max_len) {
  char *buf_ptr = usr_buf;
  ssize_t len = 0;
  for (len = 1; len < (ssize_t)max_len; len++) {
    char c = 0;
    ssize_t size = RioRead(&c, 1);
    if (size == 0) {
      if (len == 1) return 0;
      break;
    }
    if (size < 0) {
      return -1;
    }
    *buf_ptr++ = c;
    if (c == '\n') break;
  }
  *buf_ptr = '\0';
  return len;
}

ssize_t RioFileDescriptor::RioRead(char *usr_buf, ssize_t buf_size) {
  ssize_t read_size = 0;
  if ((read_size = ReadFdIntoBuf()) <= 0) return read_size;
  ssize_t size = std::min(buf_size, read_size);
  CopyBufToUsrBuf(usr_buf, size);
  return size;
}

ssize_t RioFileDescriptor::ReadFdIntoBuf() {
  if (buffer_remaining_ != 0) return buffer_remaining_;
  buffer_remaining_ = read(fd_.GetFd(), buf_, sizeof(buf_));
  if (buffer_remaining_ == -1) std::cout << "read err" << std::endl;
  if (buffer_remaining_ == 0) return 0;
  buf_pos_ptr_ = buf_;
  return buffer_remaining_;
}

void RioFileDescriptor::CopyBufToUsrBuf(char *usr_buf, ssize_t size) {
  memcpy(usr_buf, buf_pos_ptr_, size);
  buf_pos_ptr_ += size;
  buffer_remaining_ -= size;
}

RioFileDescriptor::~RioFileDescriptor() {}
