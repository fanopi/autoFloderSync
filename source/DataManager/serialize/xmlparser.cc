/*
 * xmlparse.cc
 * 代码基于 tinyxml项目
 *  Created on: 2011-5-4
 *      Author: 代码基于 TinyXML项目 重构 原作者Lee Thomason
 */

#include <ctype.h>
#include <stddef.h>

#include "xml.h"
namespace YF{
//#define DEBUG_PARSER
#if defined( DEBUG_PARSER )
#	if defined( DEBUG ) && defined( _MSC_VER )
#		include <windows.h>
#		define TIXML_LOG OutputDebugString
#	else
#		define TIXML_LOG printf
#	endif
#endif

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.	
XmlBase::Entity XmlBase::entity[NUM_ENTITY] = {
			{"&amp;",   5, '&'},
			{"&lt;",    4, '<'},
			{"&gt;",    4, '>'},
			{"&quot;",  6, '\"'},
			{"&apos;",  6, '\''}};

const YFCHAR  * XmlBase::errorString[TIXML_ERROR_STRING_COUNT] = {
			 "No error",
			 "Error",
			 "Failed to open file",
			 "Error parsing Element.",
			 "Failed to read Element name",
			 "Error reading Element value.",
			 "Error reading Attributes.",
			 "Error: empty tag.",
			 "Error reading end tag.",
			 "Error parsing Unknown.",
			 "Error parsing Comment.",
			 "Error parsing Declaration.",
			 "Error document empty.",
			 "Error null (0) or unexpected EOF found in input stream.",
			 "Error parsing CDATA.",
			 "Error when XmlDocument added to document, because XmlDocument can only be at the root.", };
// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
// Including the basic of this table, which determines the #bytes in the
// sequence from the lead byte. 1 placed for invalid sequences --
// although the result will be junk, pass it through as much as possible.
// Beware of the non-characters in UTF-8:	
//				ef bb bf (Microsoft "lead bytes")
//				ef bf be
//				ef bf bf 
const YFCHAR TIXML_UTF_LEAD_0 = 0xefU;
const YFCHAR TIXML_UTF_LEAD_1 = 0xbbU;
const YFCHAR TIXML_UTF_LEAD_2 = 0xbfU;

const int XmlBase::utf8ByteTable[256] =
{// 0	  1	  2	  3	  4	  5	  6	  7	  8	  9	  a	  b	  c	  d	  e	  f
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x00
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x10
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x20
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x30
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x40
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x50
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x60
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x70	End of ASCII range
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x80 0x80 to 0xc1 invalid
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0x90
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xa0
		1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	// 0xb0
		1,	1,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xc0 0xc2 to 0xdf 2 byte
		2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	// 0xd0
		3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	// 0xe0 0xe0 to 0xef 3 byte
		4,	4,	4,	4,	4,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1	  // 0xf0 0xf0 to 0xf4 4 byte, 0xf5 and higher invalid
};

void XmlBase::ConvertUTF32ToUTF8(unsigned long input, YFCHAR* output, int* length){
	const unsigned long BYTE_MASK = 0xBF;
	const unsigned long BYTE_MARK = 0x80;
	const unsigned long FIRST_BYTE_MARK[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

	if (input < 0x80)
		*length = 1;
	else if (input < 0x800)
		*length = 2;
	else if (input < 0x10000)
		*length = 3;
	else if (input < 0x200000)
		*length = 4;
	else{
		*length = 0;
		return;
	} // This code won't covert this correctly anyway.

	output += *length;

	// Scary fall throughs.
	switch ( *length){
		case 4:
			--output;
			*output = (YFCHAR) ((input | BYTE_MARK) & BYTE_MASK);
			input >>= 6;
		case 3:
			--output;
			*output = (YFCHAR) ((input | BYTE_MARK) & BYTE_MASK);
			input >>= 6;
		case 2:
			--output;
			*output = (YFCHAR) ((input | BYTE_MARK) & BYTE_MASK);
			input >>= 6;
		case 1:
			--output;
			*output = (YFCHAR) (input | FIRST_BYTE_MARK[ *length]);
	}
}

int XmlBase::IsAlpha(YFCHAR anyByte, XmlEncoding /*encoding*/){
	// This will only work for low-ascii, everything else is assumed to be a valid
	// letter. I'm not sure this is the best approach, but it is quite tricky trying
	// to figure out alhabetical vs. not across encoding. So take a very 
	// conservative approach.
	if (anyByte < 127)
		return isalpha(anyByte);
	else
		return 1; // What else to do? The unicode set is huge...get the english ones right.
}

int XmlBase::IsAlphaNum(YFCHAR anyByte, XmlEncoding /*encoding*/){
	if (anyByte < 127) // This will only work for low-ascii
		return isalnum(anyByte);
	else
		return 1;
}

class XmlParsingData {
		friend class XmlDocument;
	public:
		void Stamp(const YFCHAR* now, XmlEncoding encoding);
		const XmlCursor& Cursor(){
			return cursor;
		}

	private:
		XmlParsingData(const YFCHAR* start, int _tabsize, int row, int col){ // Only used by the document!
			assert(start);
			stamp = start;
			tabsize = _tabsize;
			cursor.row = row;
			cursor.col = col;
		}

		XmlCursor cursor;
		const YFCHAR* stamp;
		int tabsize;
};

void XmlParsingData::Stamp(const YFCHAR* now, XmlEncoding encoding){
	assert(now);
	if (tabsize < 1) // Do nothing if the tabsize is 0.
		return;
	int row = cursor.row; // Get the current row, column.
	int col = cursor.col;
	const YFCHAR* p = stamp;
	assert(p);

	while (p < now){
		const YFCHAR* pU = (const YFCHAR*) p; // Treat p as unsigned, so we have a happy compiler.
		switch ( *pU){
			case 0:
				return; // We *should* never get here, but in case we do, don't advance past the terminating null character, ever

			case '\r':
				++row; // bump down to the next line
				col = 0;
				++p; // Eat the character

				if ( *p == '\n') // Check for \r\n sequence, and treat this as a single character
				++p;

				break;

			case '\n':
				++row; // bump down to the next line
				col = 0;
				++p; // Eat the character

				if ( *p == '\r') // Check for \n\r sequence, and treat this as a single character.
				++p;

				break;

			case '\t':
				++p; // Eat the character
				col = (col / tabsize + 1) * tabsize; // Skip to next tab stop
				break;

			case TIXML_UTF_LEAD_0:
				if (encoding == XML_ENCODING_UTF8){
					if ( *(p + 1) && *(p + 2)){
						if ( *(pU + 1) == TIXML_UTF_LEAD_1 && *(pU + 2) == TIXML_UTF_LEAD_2) // In these cases, don't advance the column. These are 0-width spaces.
						p += 3;
						else if ( *(pU + 1) == 0xbfU && *(pU + 2) == 0xbeU) p += 3;
						else if ( *(pU + 1) == 0xbfU && *(pU + 2) == 0xbfU) p += 3;
						else{
							p += 3;
							++col;
						} // A normal character.
					}
				}else{
					++p;
					++col;
				}
				break;

			default:
				if (encoding == XML_ENCODING_UTF8){
					int step = XmlBase::utf8ByteTable[ *((const YFCHAR*) p)]; // Eat the 1 to 4 byte utf8 character.
					if (step == 0)
						step = 1; // Error case from bad encoding, but handle gracefully.
					p += step;
					++col; // Just advance one column, of course.
				}else{
					++p;
					++col;
				}
				break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	assert(cursor.row >= -1);
	assert(cursor.col >= -1);
	stamp = p;
	assert(stamp);
}

const YFCHAR* XmlBase::SkipWhiteSpace(const YFCHAR* p, XmlEncoding encoding){
	if ( !p || ! *p) return 0;

	if (encoding == XML_ENCODING_UTF8){
		while ( *p){
			const YFCHAR* pU = (const YFCHAR*) p;
			// Skip the stupid Microsoft UTF-8 Byte order marks
			if ( *(pU + 0) == TIXML_UTF_LEAD_0 && *(pU + 1) == TIXML_UTF_LEAD_1 && *(pU + 2)== TIXML_UTF_LEAD_2){
				p += 3;
				continue;
			}else if ( *(pU + 0) == TIXML_UTF_LEAD_0 && *(pU + 1) == 0xbfU && *(pU + 2) == 0xbeU){
				p += 3;
				continue;
			}else if ( *(pU + 0) == TIXML_UTF_LEAD_0 && *(pU + 1) == 0xbfU && *(pU + 2) == 0xbfU){
				p += 3;
				continue;
			}

			if (IsWhiteSpace( *p)) // Still using old rules for white space.
			++p;
			else break;
		}
	}else{
		while ( *p && IsWhiteSpace( *p))
			++p;
	}

	return p;
}

/* One of TinyXML's more performance demanding functions.
 * Try to keep the memory overhead down. The "assign" optimization removes over 10% of the execution time. */
const YFCHAR* XmlBase::ReadName(const YFCHAR* p, STRING * name, XmlEncoding encoding){
	*name = ""; // Oddly, not supported on some comilers,	name->clear(); So use this:
	assert(p);

	// Names start with letters or underscores.
	// Of course, in unicode, tinyxml has no idea what a letter *is*.
	// The algorithm is generous.
	// After that, they can be letters, underscores, numbers,hyphens, or colons.
	// (Colons are valid ony for namespaces,but tinyxml can't tell namespaces from names.)
	if (p && *p && (IsAlpha((YFCHAR) *p, encoding) || *p == '_')){
		const YFCHAR* start = p;
		while (p && *p && (IsAlphaNum((YFCHAR) *p, encoding) || *p == '_' || *p == '-' || *p
		    == '.' || *p == ':')){
			++p; // (*name) += *p;  expensive!
		}
		if (p - start > 0) name->assign(start, p - start);
		return p;
	}
	return 0;
}

const YFCHAR* XmlBase::GetEntity(const YFCHAR* p, YFCHAR* value, int* length, XmlEncoding encoding){
	// Presume an entity, and pull it out.
	STRING ent;
	int i;
	*length = 0;

	if ( *(p + 1) && *(p + 1) == '#' && *(p + 2)){
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if ( *(p + 2) == 'x'){
			if ( ! *(p + 3)) // Hexadecimal.
			return 0;

			const YFCHAR* q = p + 3;
			q = strchr(q, ';');

			if ( !q || ! *q) return 0;

			delta = q - p;
			--q;

			while ( *q != 'x'){
				if ( *q >= '0' && *q <= '9') ucs += mult * ( *q - '0');
				else if ( *q >= 'a' && *q <= 'f') ucs += mult * ( *q - 'a' + 10);
				else if ( *q >= 'A' && *q <= 'F') ucs += mult * ( *q - 'A' + 10);
				else return 0;
				mult *= 16;
				--q;
			}

		}else{
			if ( ! *(p + 2)) // Decimal.
			return 0;

			const YFCHAR* q = p + 2;
			q = strchr(q, ';');

			if ( !q || ! *q) return 0;

			delta = q - p;
			--q;

			while ( *q != '#'){
				if ( *q >= '0' && *q <= '9') ucs += mult * ( *q - '0');
				else return 0;
				mult *= 10;
				--q;
			}
		}

		if (encoding == XML_ENCODING_UTF8){
			ConvertUTF32ToUTF8(ucs, value, length); // convert the UCS to UTF-8
		}else{
			*value = (YFCHAR) ucs;
			*length = 1;
		}
		return p + delta + 1;
	}

	// Now try to match it.
	for (i = 0; i < NUM_ENTITY; ++i){
		if (strncmp(entity[i].str, p, entity[i].strLength) == 0){
			assert(strlen(entity[i].str) == entity[i].strLength);
			*value = entity[i].chr;
			*length = 1;
			return (p + entity[i].strLength);
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p; // Don't put back the last one, since we return it!
	return p + 1; // Just writes strange XML.
}

bool XmlBase::StringEqual(const YFCHAR* p, const YFCHAR* tag, bool ignoreCase, XmlEncoding encoding){
	assert(p);
	assert(tag);
	if ( !p || ! *p){
		assert(0);
		return false;
	}
	const YFCHAR* q = p;
	if (ignoreCase){
		while ( *q && *tag && ToLower( *q, encoding) == ToLower( *tag, encoding)){
			++q;
			++tag;
		}
		if ( *tag == 0){
			return true;
		}else{
			while ( *q && *tag && *q == *tag){
				++q;
				++tag;
			}
			if ( *tag == 0) // Have we found the end of the tag, and everything equal?
			return true;
		}
		return false;
	}
}

const YFCHAR* XmlBase::ReadText(const YFCHAR* p, STRING *text, bool trimWhiteSpace,
                                const YFCHAR *endTag, bool caseInsensitive, XmlEncoding encoding){
	*text = "";
	// certain tags always keep whitespace; if true, whitespace is always kept
	if ( !trimWhiteSpace || !condenseWhiteSpace){ // Keep all the white space.
		while (p && *p && !StringEqual(p, endTag, caseInsensitive, encoding)){
			int len;
			YFCHAR cArr[4] = {0, 0, 0, 0};
			p = GetChar(p, cArr, &len, encoding);
			text->append(cArr, len);
		}
	}else{
		bool whitespace = false;
		p = SkipWhiteSpace(p, encoding); // Remove leading white space:
		while (p && *p && !StringEqual(p, endTag, caseInsensitive, encoding)){
			if ( *p == '\r' || *p == '\n'){
				whitespace = true;
				++p;
			}else if (IsWhiteSpace( *p)){
				whitespace = true;
				++p;
			}else{
				if (whitespace){ // If we've found whitespace, add it before the new character. Any whitespace just becomes a space.
					( *text) += ' ';
					whitespace = false;
				}
				int len;
				YFCHAR cArr[4] = {0, 0, 0, 0};
				p = GetChar(p, cArr, &len, encoding);
				if (len == 1) ( *text) += cArr[0]; // more efficient
				else text->append(cArr, len);
			}
		}/// while
	}

	if (p && *p) p += strlen(endTag);
	return p;
}

const YFCHAR* XmlDocument::Parse(const YFCHAR* p, XmlParsingData* prevData, XmlEncoding encoding){
	ClearError( );


	/* Parse away, at the document level.
	 * Since a document contains nothing but other tags, most of what happens here is skipping white space. */
	if ( !p || ! *p){
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, XML_ENCODING_UNKNOWN);
		return 0;
	}

	/* Note that, for a document, this needs to come before the while space skip,
	 * so that parsing starts from the pointer we are given. */
	location.Clear( );
	if (prevData){
		location.row = prevData->cursor.row;
		location.col = prevData->cursor.col;
	}else{
		location.row = 0;
		location.col = 0;
	}
	XmlParsingData data(p, TabSize( ), location.row, location.col);
	location = data.Cursor( );

	if (encoding == XML_ENCODING_UNKNOWN){
		const YFCHAR* pU = (const YFCHAR*) p; // Check for the Microsoft UTF-8 lead bytes.
		if ( *(pU + 0) && *(pU + 0) == TIXML_UTF_LEAD_0 && *(pU + 1) && *(pU + 1) == TIXML_UTF_LEAD_1
		    && *(pU + 2) && *(pU + 2) == TIXML_UTF_LEAD_2){
			encoding = XML_ENCODING_UTF8;
			useMicrosoftBOM = true;
		}
	}

	p = SkipWhiteSpace(p, encoding);
	if ( !p){
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, XML_ENCODING_UNKNOWN);
		return 0;
	}

	while (p && *p){
		XmlNode* node = Identify(p, encoding);
		if (node){
			p = node->Parse(p, &data, encoding);
			LinkEndChild(node);
		}else{
			break;
		}

		if (encoding == XML_ENCODING_UNKNOWN && node->ToDeclaration( )){ // Did we get encoding info?
			XmlDeclaration* dec = node->ToDeclaration( );
			const YFCHAR* enc = dec->Encoding( );
			assert(enc);

			if ( *enc == 0)
				encoding = XML_ENCODING_UTF8;
			else if (StringEqual(enc, "UTF-8", true, XML_ENCODING_UNKNOWN))
				encoding = XML_ENCODING_UTF8;
			else if (StringEqual(enc, "UTF8", true, XML_ENCODING_UNKNOWN))
				encoding = XML_ENCODING_UTF8; // incorrect, but be nice
			else
				encoding = XML_ENCODING_LEGACY;
		}

		p = SkipWhiteSpace(p, encoding);
	}/// while

	if ( !firstChild){ // Was this empty?
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, encoding);
		return 0;
	}

	return p; // All is well.
}

void XmlDocument::SetError(int err, const YFCHAR* pError, XmlParsingData* data,
                             XmlEncoding encoding){
	if (error) // The first error in a chain is more accurate - don't set again!
	return;

	assert(err > 0 && err < TIXML_ERROR_STRING_COUNT);
	error = true;
	errorId = err;
	errorDesc = errorString[errorId];

	errorLocation.Clear( );
	if (pError && data){
		data->Stamp(pError, encoding);
		errorLocation = data->Cursor( );
	}
}

XmlNode* XmlNode::Identify(const YFCHAR* p, XmlEncoding encoding){
	XmlNode* returnNode = NULL;

	p = SkipWhiteSpace(p, encoding);
	if ( !p || ! *p || *p != '<')
		return NULL;

	p = SkipWhiteSpace(p, encoding);
	if ( !p || ! *p)
		return NULL;

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	const YFCHAR* xmlHeader = {"<?xml"};
	const YFCHAR* commentHeader = {"<!--"};
	const YFCHAR* dtdHeader = {"<!"};
	const YFCHAR* cdataHeader = {"<![CDATA["};
	if (StringEqual(p, xmlHeader, true, encoding)){
		returnNode = new XmlDeclaration( );
	}else if (StringEqual(p, commentHeader, false, encoding)){
		returnNode = new XmlComment( );
	}else if (StringEqual(p, cdataHeader, false, encoding)){
		XmlText* text = new XmlText("");
		text->SetCDATA(true);
		returnNode = text;
	}else if (StringEqual(p, dtdHeader, false, encoding)){
		returnNode = new XmlUnknown( );
	}else if (IsAlpha( *(p + 1), encoding) || *(p + 1) == '_'){
		returnNode = new XmlElement("");
	}else{
		returnNode = new XmlUnknown( );
	}

	if (returnNode) // Set the parent, so it can report errors
	returnNode->parent = this;
	return returnNode;
}

const YFCHAR* XmlElement::Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding){
	p = SkipWhiteSpace(p, encoding);
	XmlDocument* document = GetDocument( );

	if ( !p || ! *p){
		if (document)
			document->SetError(TIXML_ERROR_PARSING_ELEMENT, 0, 0, encoding);
		return 0;
	}

	if (data){
		data->Stamp(p, encoding);
		location = data->Cursor( );
	}

	// XML file's starting character MUST be '<'
	if ( *p != '<'){
		if (document)
			document->SetError(TIXML_ERROR_PARSING_ELEMENT, p, data, encoding);
		return 0;
	}

	p = SkipWhiteSpace(p + 1, encoding);


	// Read the a XML file name.
	const YFCHAR* pErr = p;
	p = ReadName(p, &value, encoding);
	if ( !p || ! *p){
		if (document)
			document->SetError(TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data, encoding);
		return 0;
	}
	STRING endTag("</");
	endTag += value;


	// Check for and read attributes. Also look for an empty tag or an end tag.
	while (p && *p){
		pErr = p;
		p = SkipWhiteSpace(p, encoding);
		if ( !p || ! *p){
			if (document)
				document->SetError(TIXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding);
			return 0;
		}


		// CASE1: an attributes tag's ending?
		if ( *p == '/'){
			++p;
			if ( *p != '>'){ // Empty tag.
				if (document)
					document->SetError(TIXML_ERROR_PARSING_EMPTY, p, data, encoding);
				return 0;
			}
			return (p + 1);


			// CASE2: Done with attributes
		}else if ( *p == '>'){
			++p;
			// Read the value (if there were any) which can include other elements
			p = ReadValue(p, data, encoding); // Note this is an Element method, and will set the error if one happens.
			if ( !p || ! *p){
				if (document) // We were looking for the end tag, but found nothing.
					document->SetError(TIXML_ERROR_READING_END_TAG, p, data, encoding);
				return 0;
			}

			// We should find the end tag now
			if (StringEqual(p, endTag.c_str( ), false, encoding)){
				p += endTag.length( );
				p = SkipWhiteSpace(p, encoding); // note that: </foo > and </foo>  are both valid end tags.
				if (p && *p && *p == '>'){
					++p;
					return p;
				}
				if (document) document->SetError(TIXML_ERROR_READING_END_TAG, p, data, encoding);
				return 0;
			}else{
				if (document) document->SetError(TIXML_ERROR_READING_END_TAG, p, data, encoding);
				return 0;
			}


			// CASE3: Try to read an attribute:
		}else{
			XmlAttribute* attrib = new XmlAttribute( );
			if ( !attrib) return 0;

			attrib->SetDocument(document);
			pErr = p;
			p = attrib->Parse(p, data, encoding);

			if ( !p || ! *p){
				if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, pErr, data, encoding);
				delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			XmlAttribute* node = attributeSet.Find(attrib->Name( ));
			if (node){
				if (document) document->SetError(TIXML_ERROR_PARSING_ELEMENT, pErr, data, encoding);
				delete attrib;
				return 0;
			}

			attributeSet.Add(attrib);
		}
	}
	return p;
}

const YFCHAR* XmlElement::ReadValue(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding){
	XmlDocument* document = GetDocument( );

	// Read in text and elements in any order.
	const YFCHAR* pWithWhiteSpace = p;
	p = SkipWhiteSpace(p, encoding);

	while (p && *p){
		// Take what we have, make a text element.
		if ( *p != '<'){
			XmlText* textNode = new XmlText("");
			if ( !textNode)
				return 0;
			if (XmlBase::IsWhiteSpaceCondensed( ))
				p = textNode->Parse(p, data, encoding);
			else // Special case: we want to keep the white space so that leading spaces aren't removed.
			p = textNode->Parse(pWithWhiteSpace, data, encoding);
			if ( !textNode->Blank( ))
				LinkEndChild(textNode);
			else
				delete textNode;

			// We hit a '<' Have we hit a new element or an end tag?
		}else{
			if (StringEqual(p, "</", false, encoding)){
				return p;
			}else{
				XmlNode* node = Identify(p, encoding);
				if (node){
					p = node->Parse(p, data, encoding);
					LinkEndChild(node);
				}else{
					return 0;
				}
			}
		}

		pWithWhiteSpace = p;
		p = SkipWhiteSpace(p, encoding);
	}

	if ( !p){
		if (document) document->SetError(TIXML_ERROR_READING_ELEMENT_VALUE, 0, 0, encoding);
	}
	return p;
}

const YFCHAR* XmlUnknown::Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding){
	XmlDocument* document = GetDocument( );
	p = SkipWhiteSpace(p, encoding);

	if (data){
		data->Stamp(p, encoding);
		location = data->Cursor( );
	}
	if ( !p || ! *p || *p != '<'){
		if (document) document->SetError(TIXML_ERROR_PARSING_UNKNOWN, p, data, encoding);
		return 0;
	}

	++p;
	value = "";
	while (p && *p && *p != '>'){
		value += *p;
		++p;
	}

	if ( !p){
		if (document)
			document->SetError(TIXML_ERROR_PARSING_UNKNOWN, 0, 0, encoding);
	}
	if ( *p == '>') return p + 1;
	return p;
}

const YFCHAR* XmlComment::Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding){
	XmlDocument* document = GetDocument( );
	value = "";

	p = SkipWhiteSpace(p, encoding);

	if (data){
		data->Stamp(p, encoding);
		location = data->Cursor( );
	}
	const YFCHAR* startTag = "<!--";
	const YFCHAR* endTag = "-->";

	if ( !StringEqual(p, startTag, false, encoding)){
		document->SetError(TIXML_ERROR_PARSING_COMMENT, p, data, encoding);
		return 0;
	}
	p += strlen(startTag);
	value = "";
	while (p && *p && !StringEqual(p, endTag, false, encoding)){ // Keep all the white space.
		value.append(p, 1);
		++p;
	}
	if (p && *p) p += strlen(endTag);

	return p;
}

const YFCHAR* XmlAttribute::Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding){
	p = SkipWhiteSpace(p, encoding);
	if ( !p || ! *p)
		return 0;

	if (data){
		data->Stamp(p, encoding);
		location = data->Cursor( );
	}
	// Read the name, the '=' and the value.
	const YFCHAR* pErr = p;
	p = ReadName(p, &name, encoding);
	if ( !p || ! *p){
		if (document)
			document->SetError(TIXML_ERROR_READING_ATTRIBUTES, pErr, data, encoding);
		return 0;
	}
	p = SkipWhiteSpace(p, encoding);
	if ( !p || ! *p || *p != '='){
		if (document)
			document->SetError(TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding);
		return 0;
	}

	++p; // skip '='
	p = SkipWhiteSpace(p, encoding);
	if ( !p || ! *p){
		if (document)
			document->SetError(TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding);
		return 0;
	}

	const YFCHAR* end;
	const YFCHAR SINGLE_QUOTE = '\'';
	const YFCHAR DOUBLE_QUOTE = '\"';

	if ( *p == SINGLE_QUOTE){
		++p;
		end = "\'"; // single quote in string
		p = ReadText(p, &value, false, end, false, encoding);
	}else if ( *p == DOUBLE_QUOTE){
		++p;
		end = "\""; // double quote in string
		p = ReadText(p, &value, false, end, false, encoding);
	}else{
		value = "";
		while (p && *p && !IsWhiteSpace( *p) && *p != '/' && *p != '>'){ // existence\whitespace\tag end
			if ( *p == SINGLE_QUOTE || *p == DOUBLE_QUOTE){ // All attribute values should be in single or double quotes.
				if (document) // But this is such a common error that the parser will try its best, even without them.
				document->SetError(TIXML_ERROR_READING_ATTRIBUTES, p, data, encoding);
				return 0;
			}
			value += *p;
			++p;
		}/// while
	}/// if
	return p;
}

const YFCHAR* XmlText::Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding){
	value = "";
	XmlDocument* document = GetDocument( );

	if (data){
		data->Stamp(p, encoding);
		location = data->Cursor( );
	}

	const YFCHAR* const startTag = "<![CDATA[";
	const YFCHAR* const endTag = "]]>";
	if (cdata || StringEqual(p, startTag, false, encoding)){
		cdata = true;

		if ( !StringEqual(p, startTag, false, encoding)){
			document->SetError(TIXML_ERROR_PARSING_CDATA, p, data, encoding);
			return 0;
		}
		p += strlen(startTag);

		// Keep all the white space, ignore the encoding, etc.
		while (p && *p && !StringEqual(p, endTag, false, encoding)){
			value += *p;
			++p;
		}
		STRING dummy;
		p = ReadText(p, &dummy, false, endTag, false, encoding);
		return p;

	}else{
		bool ignoreWhite = true;
		const YFCHAR* end = "<";
		p = ReadText(p, &value, ignoreWhite, end, false, encoding);
		if (p)
			return p - 1; // don't truncate the '<'
		return 0;
	}
}

const YFCHAR* XmlDeclaration::Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding _encoding){
	p = SkipWhiteSpace(p, _encoding);
	// Find the beginning, find the end, and look for the stuff in-between.
	XmlDocument* document = GetDocument( );
	if ( !p || ! *p || !StringEqual(p, "<?xml", true, _encoding)){
		if (document) document->SetError(TIXML_ERROR_PARSING_DECLARATION, 0, 0, _encoding);
		return 0;
	}
	if (data){
		data->Stamp(p, _encoding);
		location = data->Cursor( );
	}
	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while (p && *p){
		if ( *p == '>'){
			++p;
			return p;
		}

		p = SkipWhiteSpace(p, _encoding);
		if (StringEqual(p, "version", true, _encoding)){
			XmlAttribute attrib;
			p = attrib.Parse(p, data, _encoding);
			version = attrib.Value( );
		}else if (StringEqual(p, "encoding", true, _encoding)){
			XmlAttribute attrib;
			p = attrib.Parse(p, data, _encoding);
			encoding = attrib.Value( );
		}else if (StringEqual(p, "standalone", true, _encoding)){
			XmlAttribute attrib;
			p = attrib.Parse(p, data, _encoding);
			standalone = attrib.Value( );
		}else{ // Read over whatever it is.
			while (p && *p && *p != '>' && !IsWhiteSpace( *p))
				++p;
		}
	}
	return 0;
}

bool XmlText::Blank() const{
	for (unsigned i = 0; i < value.length( ); i++){
		if ( !IsWhiteSpace(value[i]))
			return false;
	}
	return true;
}

} /// namespace YF
