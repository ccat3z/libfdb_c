
.globl _fdb_future_get_version
.p2align	2
_fdb_future_get_version:
	adrp x8, _fdb_api_ptr_fdb_future_get_version@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_future_get_version@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_create_cluster
.p2align	2
_fdb_create_cluster:
	adrp x8, _fdb_api_ptr_fdb_create_cluster@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_create_cluster@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_cluster_create_database
.p2align	2
_fdb_cluster_create_database:
	adrp x8, _fdb_api_ptr_fdb_cluster_create_database@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_cluster_create_database@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_cluster_set_option
.p2align	2
_fdb_cluster_set_option:
	adrp x8, _fdb_api_ptr_fdb_cluster_set_option@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_cluster_set_option@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_cluster_destroy
.p2align	2
_fdb_cluster_destroy:
	adrp x8, _fdb_api_ptr_fdb_cluster_destroy@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_cluster_destroy@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_future_get_cluster
.p2align	2
_fdb_future_get_cluster:
	adrp x8, _fdb_api_ptr_fdb_future_get_cluster@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_future_get_cluster@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_future_get_database
.p2align	2
_fdb_future_get_database:
	adrp x8, _fdb_api_ptr_fdb_future_get_database@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_future_get_database@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_future_get_error
.p2align	2
_fdb_future_get_error:
	adrp x8, _fdb_api_ptr_fdb_future_get_error@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_future_get_error@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_future_is_error
.p2align	2
_fdb_future_is_error:
	adrp x8, _fdb_api_ptr_fdb_future_is_error@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_future_is_error@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_future_get_keyvalue_array
.p2align	2
_fdb_future_get_keyvalue_array:
	adrp x8, _fdb_api_ptr_fdb_future_get_keyvalue_array@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_future_get_keyvalue_array@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_transaction_get_key
.p2align	2
_fdb_transaction_get_key:
	adrp x8, _fdb_api_ptr_fdb_transaction_get_key@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_transaction_get_key@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_transaction_get_range
.p2align	2
_fdb_transaction_get_range:
	adrp x8, _fdb_api_ptr_fdb_transaction_get_range@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_transaction_get_range@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_transaction_get_range_selector
.p2align	2
_fdb_transaction_get_range_selector:
	adrp x8, _fdb_api_ptr_fdb_transaction_get_range_selector@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_transaction_get_range_selector@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_transaction_get
.p2align	2
_fdb_transaction_get:
	adrp x8, _fdb_api_ptr_fdb_transaction_get@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_transaction_get@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_setup_network
.p2align	2
_fdb_setup_network:
	adrp x8, _fdb_api_ptr_fdb_setup_network@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_setup_network@GOTPAGEOFF]
	ldr x8, [x8]
	br x8

.globl _fdb_transaction_set_option
.p2align	2
_fdb_transaction_set_option:
	adrp x8, _fdb_api_ptr_fdb_transaction_set_option@GOTPAGE
	ldr x8, [x8, _fdb_api_ptr_fdb_transaction_set_option@GOTPAGEOFF]
	ldr x8, [x8]
	br x8
