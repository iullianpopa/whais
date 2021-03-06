################################################################################
# Test module used to verify the handling of global variable description.
# 
# Iulian Popa
#
# Notes:
#  1. It contains a lot of global varibles (with long names) to make sure one
#     uses more than one comm frame to list them.
#  2. Same things for table feilds. It should facilitate the usage of multiple
#     comm frames for a full description.
################################################################################

VAR bool_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good BOOL;
VAR char_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good CHAR;
VAR date_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATE;
VAR datetime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATETIME;
VAR hirestime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good HIRESTIME;
VAR int8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT8;
VAR int16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT16;
VAR int32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT32;
VAR int64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT64;
VAR uint8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT8;
VAR uint16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT16;
VAR uint32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT32;
VAR uint64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT64;
VAR real_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good REAL;
VAR richreal_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good RICHREAL;
VAR text_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good TEXT;

VAR array_bool_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good BOOL ARRAY;
VAR array_char_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good CHAR ARRAY;
VAR array_date_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATE ARRAY;
VAR array_datetime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATETIME ARRAY;
VAR array_hirestime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good HIRESTIME ARRAY;
VAR array_int8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT8 ARRAY;
VAR array_int16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT16 ARRAY;
VAR array_int32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT32 ARRAY;
VAR array_int64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT64 ARRAY;
VAR array_uint8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT8 ARRAY;
VAR array_uint16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT16 ARRAY;
VAR array_uint32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT32 ARRAY;
VAR array_uint64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT64 ARRAY;
VAR array_real_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good REAL ARRAY;
VAR array_richreal_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good RICHREAL ARRAY;
#VAR array_text_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good TEXT ARRAY;

VAR field_bool_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good BOOL FIELD;
VAR field_char_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good CHAR FIELD;
VAR field_date_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATE FIELD;
VAR field_datetime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATETIME FIELD;
VAR field_hirestime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good HIRESTIME FIELD;
VAR field_int8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT8 FIELD;
VAR field_int16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT16 FIELD;
VAR field_int32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT32 FIELD;
VAR field_int64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT64 FIELD;
VAR field_uint8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT8 FIELD;
VAR field_uint16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT16 FIELD;
VAR field_uint32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT32 FIELD;
VAR field_uint64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT64 FIELD;
VAR field_real_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good REAL FIELD;
VAR field_richreal_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good RICHREAL FIELD;
VAR field_text_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good TEXT FIELD;

VAR field_array_bool_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good BOOL ARRAY FIELD;
VAR field_array_char_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good CHAR ARRAY FIELD;
VAR field_array_date_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATE ARRAY FIELD;
VAR field_array_datetime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good DATETIME ARRAY FIELD;
VAR field_array_hirestime_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good HIRESTIME ARRAY FIELD;
VAR field_array_int8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT8 ARRAY FIELD;
VAR field_array_int16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT16 ARRAY FIELD;
VAR field_array_int32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT32 ARRAY FIELD;
VAR field_array_int64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good INT64 ARRAY FIELD;
VAR field_array_uint8_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT8 ARRAY FIELD;
VAR field_array_uint16_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT16 ARRAY FIELD;
VAR field_array_uint32_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT32 ARRAY FIELD;
VAR field_array_uint64_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good UINT64 ARRAY FIELD;
VAR field_array_real_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good REAL ARRAY FIELD;
VAR field_array_richreal_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good RICHREAL ARRAY FIELD;
#VAR field_array_text_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good TEXT ARRAY FIELD;

VAR one_field_table_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good TABLE (field1 BOOL);
VAR complete_field_table_global_var_this_is_a_long_variable_name_suffix_coz_I_need_to_trigger_an_odd_behavior_001_good TABLE (
					bool_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad BOOL,
					char_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad CHAR,
					date_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad DATE,
					datetime_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad DATETIME,
					hirestime_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad HIRESTIME,
					int8_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT8,
					int16_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT16,
					int32_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT32,
					int64_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT64,
					uint8_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT8,
					uint16_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT16,
					uint32_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT32,
					uint64_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT64,
					real_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad REAL,
					richreal_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad RICHREAL,
					text_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad TEXT,

					array_bool_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad BOOL ARRAY,
					array_char_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad CHAR ARRAY,
					array_date_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad DATE ARRAY,
					array_datetime_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad DATETIME ARRAY,
					array_hirestime_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad HIRESTIME ARRAY,
					array_int8_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT8 ARRAY,
					array_int16_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT16 ARRAY,
					array_int32_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT32 ARRAY,
					array_int64_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad INT64 ARRAY,
					array_uint8_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT8 ARRAY,
					array_uint16_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT16 ARRAY,
					array_uint32_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT32 ARRAY,
					array_uint64_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad UINT64 ARRAY,
					array_real_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad REAL ARRAY,
					array_richreal_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad RICHREAL ARRAY);
#					array_text_field_This_is_a_long_field_suffix_coz_I_need_to_trigger_an_odd_behavior_002_bad TEXT ARRAY);

