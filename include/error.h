#ifndef DATAFRAME_ERROR_H_
    #define DATAFRAME_ERROR_H_
    typedef struct _error
    {
        char* error_type;
        char* error_msg;
    }error;
    #define RAISE(ERROR) do{printf("ERROR:\n[%s]:%s\n",ERROR.error_type,ERROR.error_msg);system("pause");system("exit");}while(0)
    error _index_invild_error;
    error _file_read_error;
    error _cell_parser_error;
    error _token_parser_error;
    error _filestream_error;
    error _cell_type_error;
    error _file_existence_error;
#endif