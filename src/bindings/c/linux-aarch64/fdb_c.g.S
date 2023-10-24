
.data
	.extern fdb_api_ptr_fdb_future_get_version
	.extern fdb_api_ptr_fdb_create_cluster
	.extern fdb_api_ptr_fdb_cluster_create_database
	.extern fdb_api_ptr_fdb_cluster_set_option
	.extern fdb_api_ptr_fdb_cluster_destroy
	.extern fdb_api_ptr_fdb_future_get_cluster
	.extern fdb_api_ptr_fdb_future_get_database
	.extern fdb_api_ptr_fdb_future_get_error
	.extern fdb_api_ptr_fdb_future_is_error
	.extern fdb_api_ptr_fdb_future_get_keyvalue_array
	.extern fdb_api_ptr_fdb_transaction_get_key
	.extern fdb_api_ptr_fdb_transaction_get_range
	.extern fdb_api_ptr_fdb_transaction_get_range_selector
	.extern fdb_api_ptr_fdb_transaction_get
	.extern fdb_api_ptr_fdb_setup_network
	.extern fdb_api_ptr_fdb_transaction_set_option

.text
	.global fdb_future_get_version
	.type fdb_future_get_version, @function
	.global fdb_create_cluster
	.type fdb_create_cluster, @function
	.global fdb_cluster_create_database
	.type fdb_cluster_create_database, @function
	.global fdb_cluster_set_option
	.type fdb_cluster_set_option, @function
	.global fdb_cluster_destroy
	.type fdb_cluster_destroy, @function
	.global fdb_future_get_cluster
	.type fdb_future_get_cluster, @function
	.global fdb_future_get_database
	.type fdb_future_get_database, @function
	.global fdb_future_get_error
	.type fdb_future_get_error, @function
	.global fdb_future_is_error
	.type fdb_future_is_error, @function
	.global fdb_future_get_keyvalue_array
	.type fdb_future_get_keyvalue_array, @function
	.global fdb_transaction_get_key
	.type fdb_transaction_get_key, @function
	.global fdb_transaction_get_range
	.type fdb_transaction_get_range, @function
	.global fdb_transaction_get_range_selector
	.type fdb_transaction_get_range_selector, @function
	.global fdb_transaction_get
	.type fdb_transaction_get, @function
	.global fdb_setup_network
	.type fdb_setup_network, @function
	.global fdb_transaction_set_option
	.type fdb_transaction_set_option, @function

.globl fdb_future_get_version
fdb_future_get_version:
	adrp x8, :got:fdb_api_ptr_fdb_future_get_version
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_future_get_version]
	ldr x8, [x8]
	br x8

.globl fdb_create_cluster
fdb_create_cluster:
	adrp x8, :got:fdb_api_ptr_fdb_create_cluster
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_create_cluster]
	ldr x8, [x8]
	br x8

.globl fdb_cluster_create_database
fdb_cluster_create_database:
	adrp x8, :got:fdb_api_ptr_fdb_cluster_create_database
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_cluster_create_database]
	ldr x8, [x8]
	br x8

.globl fdb_cluster_set_option
fdb_cluster_set_option:
	adrp x8, :got:fdb_api_ptr_fdb_cluster_set_option
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_cluster_set_option]
	ldr x8, [x8]
	br x8

.globl fdb_cluster_destroy
fdb_cluster_destroy:
	adrp x8, :got:fdb_api_ptr_fdb_cluster_destroy
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_cluster_destroy]
	ldr x8, [x8]
	br x8

.globl fdb_future_get_cluster
fdb_future_get_cluster:
	adrp x8, :got:fdb_api_ptr_fdb_future_get_cluster
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_future_get_cluster]
	ldr x8, [x8]
	br x8

.globl fdb_future_get_database
fdb_future_get_database:
	adrp x8, :got:fdb_api_ptr_fdb_future_get_database
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_future_get_database]
	ldr x8, [x8]
	br x8

.globl fdb_future_get_error
fdb_future_get_error:
	adrp x8, :got:fdb_api_ptr_fdb_future_get_error
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_future_get_error]
	ldr x8, [x8]
	br x8

.globl fdb_future_is_error
fdb_future_is_error:
	adrp x8, :got:fdb_api_ptr_fdb_future_is_error
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_future_is_error]
	ldr x8, [x8]
	br x8

.globl fdb_future_get_keyvalue_array
fdb_future_get_keyvalue_array:
	adrp x8, :got:fdb_api_ptr_fdb_future_get_keyvalue_array
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_future_get_keyvalue_array]
	ldr x8, [x8]
	br x8

.globl fdb_transaction_get_key
fdb_transaction_get_key:
	adrp x8, :got:fdb_api_ptr_fdb_transaction_get_key
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_transaction_get_key]
	ldr x8, [x8]
	br x8

.globl fdb_transaction_get_range
fdb_transaction_get_range:
	adrp x8, :got:fdb_api_ptr_fdb_transaction_get_range
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_transaction_get_range]
	ldr x8, [x8]
	br x8

.globl fdb_transaction_get_range_selector
fdb_transaction_get_range_selector:
	adrp x8, :got:fdb_api_ptr_fdb_transaction_get_range_selector
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_transaction_get_range_selector]
	ldr x8, [x8]
	br x8

.globl fdb_transaction_get
fdb_transaction_get:
	adrp x8, :got:fdb_api_ptr_fdb_transaction_get
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_transaction_get]
	ldr x8, [x8]
	br x8

.globl fdb_setup_network
fdb_setup_network:
	adrp x8, :got:fdb_api_ptr_fdb_setup_network
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_setup_network]
	ldr x8, [x8]
	br x8

.globl fdb_transaction_set_option
fdb_transaction_set_option:
	adrp x8, :got:fdb_api_ptr_fdb_transaction_set_option
	ldr x8, [x8, #:got_lo12:fdb_api_ptr_fdb_transaction_set_option]
	ldr x8, [x8]
	br x8
