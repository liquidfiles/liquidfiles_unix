# Changelog

## v2.0.0 (2026-04-06)

Updated to LiquidFiles API v4.2. This is a breaking change from the
previous API version.

### API Changes

- File upload now uses binary POST to `/message/attachments/upload?filename=X`
  instead of multipart form POST to `/attachments`
- Chunk upload uses binary POST to
  `/message/attachments/upload?filename=X&chunk=N&chunks=T`
- Delete single attachment path changed from `/attachment/{id}` to
  `/message/attachments/{id}`
- Delete message attachments now uses DELETE method instead of GET
- Filedrop file upload uses `{filedrop_url}/attachments/upload?filename=X`
- `send_email` parameter sent as boolean `true` instead of string `"true"`
- `get_api_key` handles both `{"api_key":"..."}` and
  `{"user":{"api_key":"..."}}` response formats
- HTTP redirects are now followed automatically

### Improvements

- Error messages from the server are now included in error output
  (e.g. "Server returned status 422: Invalid extension" instead of just
  "Server returned status 422")
- Removed deprecated `curl_formadd`/`curl_httppost` API (deprecated since
  curl 7.56.0 in 2017)
- Added `Accept: application/json` header to all API requests
- Added integration test script (`test.sh`)

### Bug Fixes

- Fixed attachment ID extraction (was passing raw JSON response as
  attachment ID to send/filelink requests)
- Fixed curl state leaking between requests (stale `POSTFIELDSIZE` from
  file uploads was truncating subsequent JSON request bodies)
- Fixed filedrop API key being cleared before message submission
- Fixed send response parsing (response contains raw newlines in HTML
  email body that caused JSON parse failures)
- Fixed exception catching by value (was slicing `std::exception`)

### Code Quality

- Renamed `responce` to `response` in all filenames, class names, and
  method names
- Fixed typos: `dublicate` -> `duplicate`, `Messsage` -> `Message`,
  `sucessfully` -> `successfully`, `filiename` -> `filename`
- Updated doc comments from "Xml node" to "Json object"
- Removed stale `CURLOPT_HTTPPOST` workaround

## v1.0.0

Initial JSON API implementation. Replaced the original XML-based API
with JSON using nlohmann/json for serialization.
