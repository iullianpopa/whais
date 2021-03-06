/******************************************************************************
WHAIS - An advanced database system
Copyright(C) 2014-2018  Iulian Popa

Address: Str Olimp nr. 6
         Pantelimon Ilfov,
         Romania
Phone:   +40721939650
e-mail:  popaiulian@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/


/* careful whit this to be the same as in
 * whais.y */
#ifndef YYSTYPE
#define YYSTYPE struct SemValue*
#endif

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

/* Include this first to avoid type redefinitions
 * when the Visual C++ compiler is used. */
struct ParserState;
#include "whais.tab.h"

#include "whais.h"
#include "utils/wutf.h"
#include "compiler/wopcodes.h"
#include "parser.h"
#include "../semantics/wlog.h"


#define MAX_KEYWORD_LEN          13 /* The longest string length of a keyword. */
#define COMPOSED_OPERATOR_LEN    2  /* Thestring length of composed operator. */


typedef struct
{
  const char* text;
  uint_t      token;
} TOKEN_SEMANTIC;

/* Identify the token type. */
typedef enum
{
  TK_CHAR,         /* A character constant. (e.g 'a') */
  TK_STRING,       /* A string constant. (e.g. "String"). */
  TK_TIME,         /* Date /moment constant. */
  TK_IDENTIFIER,   /* An identifier */
  TK_NUMERIC,      /* An integer constant.  */
  TK_OPERATOR,     /* Expression operator(e.g. '+' , '-', '<='). */
  TK_REAL,         /* A real number */
  TK_KEYWORD,      /* Keyword. */
  TK_UNDETERMINED, /* Undetermined token type. */
  TK_ERROR         /* Error signaler. */
} TOKEN_TYPE;


static TOKEN_SEMANTIC sgKeywords[] = {
                                        {"AND", AND},
                                        {"ARRAY", ARRAY},
                                        {"BOOL", BOOL},
                                        {"BREAK", BREAK},
                                        {"CHAR", CHAR},
                                        {"CONTINUE", CONTINUE},
                                        {"DATE", DATE},
                                        {"DATETIME", DATETIME},
                                        {"DO", DO},
                                        {"ELSE", ELSE},
                                        {"END", END},
                                        {"ENDPROC", ENDPROC},
                                        {"ENDSYNC", ENDSYNC},
                                        {"EXTERN", EXTERN},
                                        {"FALSE", W_FALSE},
                                        {"FIELD", FIELD},
                                        {"FOR", FOR},
                                        {"HIRESTIME", HIRESTIME},
                                        {"IF", IF},
                                        {"INT8", INT8},
                                        {"INT16", INT16},
                                        {"INT32", INT32},
                                        {"INT64", INT64},
                                        {"OR", OR},
                                        {"NOT", NOT},
                                        {"NULL", WHAIS_NULL},
                                        {"REAL", REAL},
                                        {"RETURN", RETURN},
                                        {"RICHREAL", RICHREAL},
                                        {"PROCEDURE", PROCEDURE},
                                        {"SYNC", SYNC},
                                        {"TABLE", TABLE},
                                        {"TEXT", TEXT},
                                        {"TRUE", W_TRUE},
                                        {"UINT8", UINT8},
                                        {"UINT16", UINT16},
                                        {"UINT32", UINT32},
                                        {"UINT64", UINT64},
                                        {"UNDEFINED", UNDEFINED},
                                        {"UNTIL", UNTIL},
                                        {"VAR", VAR},
                                        {"WHILE", WHILE},
                                        {"XOR", XOR},

                                        /* some aliases of our own */
                                        {"NUMBER", INT32},
                                        {NULL, 0}
                                    };

static TOKEN_SEMANTIC sgMultiCharOps[] = {
                                           {"==", EQ},
                                           {"!=", NE},
                                           {"<=", LE},
                                           {">=", GE},
                                           {"+=", SADD},
                                           {"-=", SSUB},
                                           {"*=", SMUL},
                                           {"/=", SDIV},
                                           {"%=", SMOD},
                                           {"&=", SAND},
                                           {"^=", SXOR},
                                           {"|=", SOR},
                                           {"&&", AND},
                                           {"||", OR},
                                           {NULL, 0}
                                         };


INLINE static bool_t
is_numeric(const char c, const bool_t isHex)
{
  return isdigit(c)
         || (isHex
             && ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')));
}

INLINE static bool_t
is_idlegal(const char c)
{
  return is_numeric(c, FALSE) || isalpha(c) || (c == '_');
}

INLINE static bool_t
is_eol(const char c)
{
  return(c == 0x0A || c == 0x0D);
}

/* Read the next token from buffer (ignores any white spaces). */
static TOKEN_TYPE
next_token(const char     *buffer,
           char const    **outpToken,
           uint_t* const   outTokenLen)
{
  TOKEN_TYPE result = TK_UNDETERMINED;

  assert(outpToken != NULL);
  assert(outTokenLen != NULL);

  while (isspace(*buffer))
    ++buffer;

  if (*buffer == '#')
  {
    /* skip line with commentaries and try again */
    while (!is_eol( *buffer))
      ++buffer;

    return next_token(buffer, outpToken, outTokenLen);
  }

  *outpToken   = buffer;
  *outTokenLen = 0;

  if ((*buffer == '-') && is_numeric(buffer[1], FALSE))
    buffer++;

  if (is_numeric( *buffer, FALSE))
  {
    bool_t isHexa = FALSE;

    if (buffer[0] == '0'
        && (buffer[1] == 'x' || buffer[1] == 'X'))
    {
      isHexa  = TRUE;
      buffer += 2;
    }

    while (is_numeric( *buffer, isHexa))
      buffer++;

    result = TK_NUMERIC;
    if (*buffer == '.' && ! isHexa)
    {
      ++buffer;

      while (is_numeric(*buffer, FALSE))
        ++buffer;

      result = TK_REAL;
    }

    if (result == TK_NUMERIC
        && (*buffer == 's'
            || *buffer == 'S'
            || *buffer == 'u'
            || *buffer == 'U'))
    {
      ++buffer;
    }
  }
  else if (is_idlegal( *buffer))
  {
    while (is_idlegal( *buffer))
      buffer++;

    result = TK_IDENTIFIER;
  }
  else if ((*buffer == '<' || *buffer == '>' || *buffer == '!')
           || (*buffer == '+' || *buffer == '-')
           || (*buffer == '*' || *buffer == '/' || *buffer == '%')
           || (*buffer == '&' || *buffer == '^' || *buffer == '|')
           || (*buffer == '='))
  {
    const char oldCh = *buffer++;

    if (*buffer == oldCh || *buffer == '=')
      buffer++;

    result = TK_OPERATOR;
  }
  else if (*buffer == '\"')
  {
    do
    {
      buffer++;

      if (*buffer == '\\')
        buffer += 2;

      if (is_eol(*buffer) || *buffer == 0)
      {
        /* an new line or end of buffer
         * encountered before ending \" */
        return TK_ERROR;
      }
    }
    while (*buffer != '\"');

    buffer++;
    result = TK_STRING;
  }
  else if (*buffer == '\'')
  {
    uint_t entrySize = 1;
    uint_t dummy;

    do
    {
      buffer++;

      if (*buffer == '\\')
        buffer += 2, ++entrySize;

      if (is_eol(*buffer) || *buffer == 0)
      {
        /* an new line or end of buffer
         * encountered before ending \' */
        return TK_ERROR;
      }
    }
    while (*buffer != '\'');

    buffer++;

    if (((*outpToken)[1] == '\\')
        && is_numeric((*outpToken)[entrySize], FALSE))
    {
      bool_t hexa = FALSE;

      if (((*outpToken)[entrySize++] == '0')
          && (((*outpToken)[entrySize] == 'x')
              || ((*outpToken)[entrySize] == 'X')))
      {
        ++entrySize;
        hexa = TRUE;
      }

      while (is_numeric((*outpToken)[entrySize], hexa))
        ++entrySize;
    }
    else
      entrySize += wh_load_utf8_cp((const uint8_t*)(*outpToken + entrySize), &dummy);

    result = ((*outpToken)[entrySize] == '\'') ? TK_CHAR : TK_TIME;
  }
  else
    buffer++;

  *outTokenLen = (uint_t)(buffer - (*outpToken));

  return result;
}

/* Get the semantic of specified keyword. */
static int
parse_keyword(const char* keyword, uint_t keyLen)
{
  uint_t i;
  char   normalKey[MAX_KEYWORD_LEN];

  if (keyLen >= MAX_KEYWORD_LEN)
    return 0;

  /* Normalize the key, by conveting to the uppercase form. */
  for (i = 0; i < keyLen; i++)
    normalKey[i] = toupper(keyword[i]);

  normalKey[i] = 0; /* add NULL at the end */

  /* Now search the key. */
  i = 0;
  while (sgKeywords[i].text != NULL)
  {
    if (strcmp(sgKeywords[i].text, normalKey) == 0)
      break;

    ++i;
  }
  return sgKeywords[i].token;
}

static int
parse_multichar_operator(const char* const op)
{
  uint_t i = 0;

  while (sgMultiCharOps[i].text != NULL)
  {
    if (strncmp( sgMultiCharOps[i].text, op, COMPOSED_OPERATOR_LEN) == 0)
      break;

    ++i;
  }

  return sgMultiCharOps[i].token;
}


static uint_t
parse_integer(const char       *buffer,
              uint_t            bufferLen,
              const bool_t      checkForUnsigned,
              uint64_t* const   outValue,
              bool_t* const     outSigned)
{
  const uint_t oldLen = bufferLen;

  uint_t base     = 10;
  bool_t negative = FALSE;

  assert(buffer != NULL);
  assert(bufferLen != 0);
  assert(outValue != NULL);

  *outValue = 0;
  if (buffer[0] == '-'
      && bufferLen > 1
      && is_numeric(buffer[1], FALSE))
  {
    negative = *outSigned = TRUE;
    ++buffer, --bufferLen;
  }

  if ((buffer[0] == '0')
      && bufferLen > 1
      && (buffer[1] == 'x' || buffer[1] == 'X'))
  {
    base = 16; /* hexa decimal notation */
    buffer +=2, bufferLen -= 2;
  }
  else if (buffer[0] == '0')
    ++buffer, --bufferLen;

  while (bufferLen > 0)
  {
    uint8_t digit;

    if (*buffer >= '0' && *buffer <= '9')
      digit = (*buffer - '0');

    else if (base == 16)
    {
      if (*buffer >= 'a' && *buffer <= 'f')
        digit = ((*buffer - 'a') + 10);

      else if (*buffer >= 'A' && *buffer <= 'F')
        digit = ((*buffer - 'A') + 10);

      else
        break;
    }
    else
      break;

    *outValue *= base;
    *outValue += digit;

    ++buffer, --bufferLen;
  }

  if (negative)
    *outValue *= -1;

  if (checkForUnsigned
      && (*buffer == 'u' || *buffer == 'U'))
  {
    --bufferLen, *outSigned = FALSE;
  }
  else if (checkForUnsigned
          && (*buffer == 's' || *buffer == 'S'))
  {
    --bufferLen, *outSigned = TRUE;
  }
  else
    *outSigned = negative;

  return (oldLen - bufferLen);
}

static uint_t
parse_real(const char       *buffer,
           uint_t            bufferLen,
           struct SemCReal  *outReal)
{
  const uint_t oldLen = bufferLen;

  int64_t precision         = 1;
  bool_t  foundDecimalPoint = FALSE;
  bool_t  negative          = FALSE;

  assert(buffer != NULL);
  assert(bufferLen != 0);
  assert(outReal != NULL);

  outReal->integerPart    = 0;
  outReal->fractionalPart = 0;

  if (buffer[0] == '-'
      && bufferLen > 1
      && is_numeric(buffer[1], FALSE))
  {
    negative = TRUE;
    ++buffer, --bufferLen;
  }

  while (bufferLen > 0)
  {
    int digit = 0;

    if (*buffer >= '0' && *buffer <= '9')
      {
        digit += (*buffer - '0');
        if (! foundDecimalPoint)
        {
          outReal->integerPart *= 10;
          outReal->integerPart += digit;
        }
        else
        {
          precision *= 10;
          if (precision < 0 || W_LDRR_PRECISSION <= precision)
            break;

          outReal->fractionalPart *= 10;
          outReal->fractionalPart += digit;
        }
      }
    else if (*buffer == '.')
    {
      if (foundDecimalPoint)
        return 0; /* We already found a decimal point. */

      foundDecimalPoint = TRUE;
    }
    else
      break;

    ++buffer, --bufferLen;
  }

  for (; precision < W_LDRR_PRECISSION; precision *= 10)
    outReal->fractionalPart *= 10;

  if (negative)
  {
    outReal->integerPart    = -outReal->integerPart;
    outReal->fractionalPart = -outReal->fractionalPart;
  }

  return (oldLen - bufferLen);
}

static uint_t
parse_character(const char  *buffer,
                uint_t       bufferLen,
                uint32_t    *outChar)
{
  uint_t result = 0;

  assert(buffer != NULL);
  assert(bufferLen != 0);
  assert(outChar != NULL);

  if (*buffer == '\\')
  {
    if (bufferLen > 0)
      ++buffer, --bufferLen, ++result;

    else
      return 0; /* error */

    if (*buffer == '\\')
    {
      *outChar = '\\';
      ++result;
    }
    else if (*buffer == 'n')
    {
      *outChar = '\n';
      ++result;
    }
    else if (*buffer == 'r')
    {
      *outChar = '\r';
      ++result;
    }
    else if (*buffer == 'f')
    {
      *outChar = '\f';
      ++result;
    }
    else if (*buffer == 't')
    {
      *outChar = '\t';
      ++result;
    }
    else if (*buffer == 'v')
    {
      *outChar = '\v';
      ++result;
    }
    else if (*buffer == 'b')
    {
      *outChar = '\b';
      ++result;
    }
    else if (*buffer == 'a')
    {
      *outChar = '\a';
      ++result;
    }
    else if (*buffer == '\'')
    {
      *outChar = '\'';
      ++result;
    }
    else if (*buffer == '"')
    {
      *outChar = '"';
      ++result;
    }
    else if (is_numeric(*buffer, FALSE))
    {
      uint64_t value = 0;
      bool_t   dummy = FALSE;

      result += parse_integer(buffer, bufferLen, FALSE, &value, &dummy);
      if (value > 0xFFFFFFFF)
        return 0;

      *outChar = value;
    }
  }
  else
    result = wh_load_utf8_cp((uint8_t*)buffer, outChar);

  return result;
}

static uint_t
parse_string(const char  *buffer,
             uint_t       bufferLen,
             char*        destination,
             uint_t      *outDestinationLen)
{
  const uint_t oldLen = bufferLen;

  assert(buffer != NULL);
  assert(bufferLen != 0);
  assert(destination != NULL);

  *outDestinationLen = 0;

  while (bufferLen > 0 && *buffer != '\"')
  {
    uint32_t currChar = 0;
    uint_t   result;

    if (buffer[0] == '\\')
      result = parse_character(buffer, bufferLen, &currChar);

    else
      currChar = buffer[0], result = 1;


    /* Currently there is support for Unicode only through UTF-8.
       Cannot handle trough escape chars the full range of Unicode. */
    if (result != 0
        || currChar <= 0xFF)
      {
        buffer    += result,
        bufferLen -= result;

        *destination++      = currChar;
        *outDestinationLen += 1;
      }
    else
      return 0;
  }

  *destination        = 0;      /* add the null character */
  *outDestinationLen += 1;

  return(oldLen - bufferLen);
}

static uint_t
parse_time_value(const char       *buffer,
                 uint_t            bufferLen,
                 struct SemCTime  *outTime)
{

  uint_t intValLen = 0;
  uint_t result    = 0;

  int64_t intVal;
  bool_t  dummy;

  /* Initialize the structure with default valid values */
  memset(outTime, 0, sizeof(outTime[0]));
  outTime->month = outTime->day = 1;

  /* found the year part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t*)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->year = (int16_t) intVal;
  }
  else
    return 0;   /* parsing error */

  if (buffer[0] == '\'')
    return result;      /* end of date/time entry */

  else if ((buffer[0] != '/') && (buffer[0] != '-'))
    return 0; /* parsing error - no date delimiter */

  else
    ++result, ++buffer, --bufferLen;

  /* found the month part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t*)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->month = (uint8_t) intVal;
  }
  else
    return 0;   /* parsing error */

  if (buffer[0] == '\'')
    return result;      /* end of date/time entry */

  else if ((buffer[0] != '/') && (buffer[0] != '-'))
    return 0; /* parsing error - no date delimiter */

  else
    ++result, ++buffer, --bufferLen;

  /* found the day part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t *)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->day = (uint8_t) intVal;
  }
  else
    return 0;   /* parsing error */

  if (buffer[0] == '\'')
    return result;      /* end of date/time entry */

  else if (buffer[0] != ' ')
    return 0; /* parsing error - no date delimiter */
  else
    ++result, ++buffer, --bufferLen;

  /* found the hour part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t *)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->hour = (uint8_t) intVal;
  }
  else
    return 0;   /* parsing error */

  if (buffer[0] != ':')
      return 0; /* parsing error - no time delimiter. */

  else
    ++result, ++buffer, --bufferLen;

  /* found the minute part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t *)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->min = (uint8_t) intVal;
  }
  else
    return 0;   /* parsing error */

  if (buffer[0] == '\'')
    return result;

  else if (buffer[0] != ':')
      return 0; /* parsing error - no time delimiter. */

  else
    ++result, ++buffer, --bufferLen;

  /* found the second part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t *)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->sec = (uint8_t) intVal;
  }
  else
    return 0;   /* parsing error */

  if (buffer[0] == '\'')
    return result;

  else if (buffer[0] != '.')
      return 0; /* parsing error - no time delimiter. */

  else
    ++result, ++buffer, --bufferLen;

  /* found the microsecond part */
  intValLen = parse_integer(buffer, bufferLen, FALSE, (uint64_t *)&intVal, &dummy);
  if (intValLen > 0)
  {
    result    += intValLen;
    buffer    += intValLen;
    bufferLen -= intValLen;

    outTime->usec = (uint32_t) intVal;
  }
  else
    return 0;   /* parsing error */

  return result;
}

int
yylex(YYSTYPE * lvalp, struct ParserState* parser)
{
  int          result    = 0;
  const char  *buffer    = parser->buffer;
  const char  *pToken    = NULL;
  uint_t       tokenLen  = 0;
  TOKEN_TYPE   tokenType = TK_ERROR;
  uint_t       bufferOff = parser->bufferPos;

  if (parser->bufferPos > parser->bufferSize)
    return 0;

  /* Recall where to start from */
  buffer    += bufferOff;
  tokenType  = next_token(buffer, &pToken, &tokenLen);

  /* Start from here next call. */
  parser->bufferPos += (uint_t) ((pToken + tokenLen) - buffer);

  /* Allocate storage for semantic value, if the token type demands it. */
  *lvalp = NULL;
  if (tokenType != TK_UNDETERMINED
      && tokenType != TK_OPERATOR
      && (tokenType != TK_IDENTIFIER
          || (result = parse_keyword(pToken, tokenLen)) == 0))
  {
    *lvalp = alloc_sem_value(parser);
    if (*lvalp == NULL)
    {
      log_message(parser, IGNORE_BUFFER_POS, parser->bufferPos,  MSG_NO_MEM);
      return 0; /* error */
    }
  }
  else if (tokenType == TK_IDENTIFIER)
  {
    /* after we parsed the token we found is a keyword */
    tokenType = TK_KEYWORD;
    return result;
  }
  else if (tokenLen == 1)
    return *pToken;

  /* Initialize the semantic value, based on the token type. */
  switch(tokenType)
  {
    case TK_IDENTIFIER:
      /* parsing was successful */
      (*lvalp)->val_type        = VAL_ID;
      (*lvalp)->val.u_id.name   = pToken;
      (*lvalp)->val.u_id.length = tokenLen;

      result = IDENTIFIER;
      break;

    case TK_OPERATOR:
      return parse_multichar_operator(pToken);

    case TK_NUMERIC:
      result = parse_integer(pToken,
                             tokenLen,
                             TRUE,
                             &(*lvalp)->val.u_int.value,
                             &(*lvalp)->val.u_int.isSigned);
      if (result != 0)
      {
        (*lvalp)->val_type = VAL_C_INT;
        result = WHAIS_INTEGER;
      }
      break;

    case TK_CHAR:
      if ((tokenLen > 2) || (pToken[0] != '\''))
      {
        pToken++;
        result = parse_character(pToken, tokenLen - 1, &((*lvalp)->val.u_char.value));
        if (pToken[result] != '\'')
          result = 0; /* error */
      }
      else
        result = 0; /* error */

      if (result != 0)
      {
        (*lvalp)->val_type = VAL_C_CHAR;
        result = WHAIS_CHAR;
      }
      break;

    case TK_STRING:
      if ((tokenLen > 1) || (pToken[0] != '\"'))
      {
        pToken++;
        (*lvalp)->val.u_text.text = alloc_str(parser->strings, tokenLen - 1);
        result = parse_string(pToken,
                              tokenLen,
                              (char* )(*lvalp)->val.u_text.text,
                              &(*lvalp)->val.u_text.length);
        if (pToken[result] != '\"')
          result = 0;
        else
        {
          (*lvalp)->val_type = VAL_C_TEXT;
          result = WHAIS_TEXT;
        }
      }
      else
        result = 0; /* error */

      break;

    case TK_TIME:
      if ((tokenLen > 2) || (pToken[0] != '\''))
      {
        pToken++;
        result = parse_time_value(pToken, tokenLen - 1, &(*lvalp)->val.u_time);
        if (pToken[result] != '\'')
          result = 0;
      }
      else
        result = 0; /* error */

      if (result != 0)
      {
        (*lvalp)->val_type = VAL_C_TIME;
        result = WHAIS_TIME;
      }
      break;

    case TK_REAL:
      result = parse_real(pToken, tokenLen, &(*lvalp)->val.u_real);
      if (result != 0)
      {
        (*lvalp)->val_type = VAL_C_REAL;
        result = WHAIS_REAL;
      }
      break;

    default:
      result = 0; /* error */
  }

  return result;
}

/* This is internally used by yyparse(), it's declaration is found on wisper.y */
int
yyerror(struct ParserState* parser, const char* msg)
{
  char excerpt[128] = {0, };

  uint_t i, lastTokenOffset = parser->bufferPos;
  while ((lastTokenOffset > 0) && ! isspace(parser->buffer[lastTokenOffset]))
    --lastTokenOffset;

  for (i = 0; i < sizeof(excerpt) && lastTokenOffset + i < parser->bufferSize; ++i)
    {
      excerpt[i] = parser->buffer[lastTokenOffset + i];
      if ((excerpt [i] == '\n') || (excerpt[i] == '#'))
        {
          excerpt[i] = 0;
          break;
        }
    }

  log_message(parser, parser->bufferPos, MSG_COMPILER_ERR, excerpt);

  return 0;
}

