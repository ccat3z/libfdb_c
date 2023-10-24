void fdb_api_ptr_unimpl() { fprintf(stderr, "UNIMPLEMENTED FDB API FUNCTION\n"); abort(); }

void fdb_api_ptr_removed() { fprintf(stderr, "REMOVED FDB API FUNCTION\n"); abort(); }

void* fdb_api_ptr_fdb_future_get_version = (void*)&fdb_api_ptr_unimpl;
#define fdb_future_get_version_v620_PREV fdb_future_get_version_v619
void* fdb_api_ptr_fdb_create_cluster = (void*)&fdb_api_ptr_unimpl;
#define fdb_create_cluster_v610_PREV fdb_create_cluster_v609
void* fdb_api_ptr_fdb_cluster_create_database = (void*)&fdb_api_ptr_unimpl;
#define fdb_cluster_create_database_v610_PREV fdb_cluster_create_database_v609
void* fdb_api_ptr_fdb_cluster_set_option = (void*)&fdb_api_ptr_unimpl;
#define fdb_cluster_set_option_v610_PREV fdb_cluster_set_option_v609
void* fdb_api_ptr_fdb_cluster_destroy = (void*)&fdb_api_ptr_unimpl;
#define fdb_cluster_destroy_v610_PREV fdb_cluster_destroy_v609
void* fdb_api_ptr_fdb_future_get_cluster = (void*)&fdb_api_ptr_unimpl;
#define fdb_future_get_cluster_v610_PREV fdb_future_get_cluster_v609
void* fdb_api_ptr_fdb_future_get_database = (void*)&fdb_api_ptr_unimpl;
#define fdb_future_get_database_v610_PREV fdb_future_get_database_v609
void* fdb_api_ptr_fdb_future_get_error = (void*)&fdb_api_ptr_unimpl;
#define fdb_future_get_error_v23_PREV fdb_future_get_error_v22
void* fdb_api_ptr_fdb_future_is_error = (void*)&fdb_api_ptr_unimpl;
#define fdb_future_is_error_v23_PREV fdb_future_is_error_v22
void* fdb_api_ptr_fdb_future_get_keyvalue_array = (void*)&fdb_api_ptr_unimpl;
#define fdb_future_get_keyvalue_array_v14_PREV fdb_future_get_keyvalue_array_v13
void* fdb_api_ptr_fdb_transaction_get_key = (void*)&fdb_api_ptr_unimpl;
#define fdb_transaction_get_key_v14_PREV fdb_transaction_get_key_v13
void* fdb_api_ptr_fdb_transaction_get_range = (void*)&fdb_api_ptr_unimpl;
#define fdb_transaction_get_range_v14_PREV fdb_transaction_get_range_v13
void* fdb_api_ptr_fdb_transaction_get_range_selector = (void*)&fdb_api_ptr_unimpl;
#define fdb_transaction_get_range_selector_v14_PREV fdb_transaction_get_range_selector_v13
void* fdb_api_ptr_fdb_transaction_get = (void*)&fdb_api_ptr_unimpl;
#define fdb_transaction_get_v14_PREV fdb_transaction_get_v13
void* fdb_api_ptr_fdb_setup_network = (void*)&fdb_api_ptr_unimpl;
#define fdb_setup_network_v14_PREV fdb_setup_network_v13
void* fdb_api_ptr_fdb_transaction_set_option = (void*)&fdb_api_ptr_unimpl;
#define fdb_transaction_set_option_v14_PREV fdb_transaction_set_option_v13
