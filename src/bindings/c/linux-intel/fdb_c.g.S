.intel_syntax noprefix

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
	mov r11, qword ptr [fdb_api_ptr_fdb_future_get_version@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_create_cluster
fdb_create_cluster:
	mov r11, qword ptr [fdb_api_ptr_fdb_create_cluster@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_cluster_create_database
fdb_cluster_create_database:
	mov r11, qword ptr [fdb_api_ptr_fdb_cluster_create_database@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_cluster_set_option
fdb_cluster_set_option:
	mov r11, qword ptr [fdb_api_ptr_fdb_cluster_set_option@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_cluster_destroy
fdb_cluster_destroy:
	mov r11, qword ptr [fdb_api_ptr_fdb_cluster_destroy@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_future_get_cluster
fdb_future_get_cluster:
	mov r11, qword ptr [fdb_api_ptr_fdb_future_get_cluster@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_future_get_database
fdb_future_get_database:
	mov r11, qword ptr [fdb_api_ptr_fdb_future_get_database@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_future_get_error
fdb_future_get_error:
	mov r11, qword ptr [fdb_api_ptr_fdb_future_get_error@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_future_is_error
fdb_future_is_error:
	mov r11, qword ptr [fdb_api_ptr_fdb_future_is_error@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_future_get_keyvalue_array
fdb_future_get_keyvalue_array:
	mov r11, qword ptr [fdb_api_ptr_fdb_future_get_keyvalue_array@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_transaction_get_key
fdb_transaction_get_key:
	mov r11, qword ptr [fdb_api_ptr_fdb_transaction_get_key@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_transaction_get_range
fdb_transaction_get_range:
	mov r11, qword ptr [fdb_api_ptr_fdb_transaction_get_range@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_transaction_get_range_selector
fdb_transaction_get_range_selector:
	mov r11, qword ptr [fdb_api_ptr_fdb_transaction_get_range_selector@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_transaction_get
fdb_transaction_get:
	mov r11, qword ptr [fdb_api_ptr_fdb_transaction_get@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_setup_network
fdb_setup_network:
	mov r11, qword ptr [fdb_api_ptr_fdb_setup_network@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11

.globl fdb_transaction_set_option
fdb_transaction_set_option:
	mov r11, qword ptr [fdb_api_ptr_fdb_transaction_set_option@GOTPCREL+rip]
	mov r11, qword ptr [r11]
	jmp r11
