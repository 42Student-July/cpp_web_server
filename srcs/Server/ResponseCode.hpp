#ifndef SRCS_SERVER_RESPONSECODE_HPP_
#define SRCS_SERVER_RESPONSECODE_HPP_
enum ResponseCode {
  kKk200Ok = 200,
  kKk201Created = 201,
  kKk204NoContent = 204,
  kKk400BadRequest = 400,
  kKk401Unauthorized = 401,
  kKk403Forbidden = 403,
  kKk404NotFound = 404,
  kKk405MethodNotAllowed = 405,
  kKk413RequestEntityTooLarge = 413,
  kKk429TooManyRequest = 429,
  kKk500internalServerError = 500,
  kKk501NotImplemented = 501,
  kKk503ServerUnavailable = 503,
  kKk505HTTPVersionNotSupported = 505,
  kKkNotSet = 0
};
#endif  // SRCS_SERVER_RESPONSECODE_HPP_
