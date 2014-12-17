################################################################################
# Test module used to verify the correct execution of field related functions
# provided by the native base library.
# 
# Iulian Popa
#
################################################################################


#@include whais_std.wh

PROCEDURE test_field_sort (t AS TABLE, f AS UINT64, reverse AS BOOL)
RETURN TABLE
DO
	field_sort_table (table_field_by_id (t, f), reverse);

	RETURN t;
ENDPROC


PROCEDURE test_field_mimim (t AS TABLE, f AS UINT64)
RETURN UNDEFINED
DO
	return field_smallest (table_field_by_id (t, f));
ENDPROC


PROCEDURE test_field_maxim (t AS TABLE, f AS UINT64)
RETURN UNDEFINED
DO
	return field_biggest (table_field_by_id (t, f));
ENDPROC


PROCEDURE test_field_average (t AS TABLE, f AS UINT64)
RETURN RICHREAL
DO
	return field_average (table_field_by_id (t, f));
ENDPROC


PROCEDURE test_field_name (t AS TABLE, f AS UINT64)
RETURN TEXT
DO
	return field_name (table_field_by_id (t, f));
ENDPROC


PROCEDURE test_field_match (t		AS TABLE, 
							f		AS UINT64,
						    min		AS UNDEFINED,
						    max		AS UNDEFINED,
						    fromRow AS UINT64,
						    toRow   AS UINT64)
RETURN ARRAY OF UINT64
DO
	return match_rows (table_field_by_id (t, f), 
	                   min,
                       max,
                       fromRow,
                       toRow);
ENDPROC

