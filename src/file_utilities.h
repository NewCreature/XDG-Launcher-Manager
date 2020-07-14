#ifndef XLM_FILE_UTILITIES_H
#define XLM_FILE_UTILITIES_H

const char * xlm_get_filename_extension(const char * fn);
const char * xlm_get_next_filename(const char * path, const char * base_name, const char * extension);
bool xlm_copy_file(const char * fn, const char * dest_fn);

#endif
