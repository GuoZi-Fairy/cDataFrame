#include "error.h"
    error _index_invild_error = {"index error","The index may "};
    error _file_read_error = {"file read error","catch a error when reading the file"};
    error _cell_parser_error = {"cell parse error","please check your data,have a invild token:{\'\\r\',\'\\n\'"};
    error _token_parser_error = {"token parse error","something wrong in the \',\'"};
    error _filestream_error = {"file colse error","the file stream colse unsuccessed"};
    error _cell_type_error = {"Cell type invild","please check this cell's type"};
    error _file_existence_error = {"file does not exist","please check the file"};