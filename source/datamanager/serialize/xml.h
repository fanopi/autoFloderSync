/*
 * xml.h
 *
 *  Created on: 2011-5-4
 *      Author: ������� TinyXML��Ŀ �ع� ԭ����Lee Thomason
 */
#ifndef TINYXML_INCLUDED
#define TINYXML_INCLUDED

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4530 )
#pragma warning( disable : 4786 )
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../include/typedef.h"
#include "../../include/support/datastruct/string.hpp"
#include "../../include/support/datastruct/tree.hpp"
#define STRING		String

#define TIXML_SAFE
#ifdef TIXML_SAFE
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
#define TIXML_SNPRINTF _snprintf_s
#define TIXML_SSCANF   sscanf_s
#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define TIXML_SNPRINTF _snprintf
#define TIXML_SSCANF   sscanf
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define TIXML_SNPRINTF snprintf
#define TIXML_SSCANF   sscanf
#else
#define TIXML_SNPRINTF snprintf
#define TIXML_SSCANF   sscanf
#endif
#endif

namespace YF{

class XmlDocument;
class XmlElement;
class XmlComment;
class XmlUnknown;
class XmlAttribute;
class XmlText;
class XmlDeclaration;
class XmlParsingData;

/*	Internal structure for tracking location of items in the XML file.*/
struct XmlCursor {
		XmlCursor() {	Clear( );}
		void Clear() { row = col = -1;}
		int row; // 0 based.
		int col; // 0 based.
};

/* Implements the interface to the "Visitor pattern" (see the Accept() method.)
 * If you call the Accept() method, it requires being passed a XmlVisitor class to handle callbacks.
 * For nodes that contain other nodes (Document, Element) you will get called with a VisitEnter/VisitExit pair.
 * Nodes that are always leaves are simply called with Visit().
 * If you return 'true' from a Visit method, recursive parsing will continue.
 * If you return false, NO children of this node or its sibilings will be Visited.
 * All flavors of Visit methods have a default implementation that returns 'true' (continue visiting).
 * You need to only override methods that are interesting to you.
 * Generally Accept() is called on the XmlDocument, although all nodes suppert Visiting.
 * You should never change the document from a callback.
 * @sa XmlNode::Accept()*/
class XmlVisitor {
	public:
		virtual ~XmlVisitor(){}

		virtual bool VisitEnter(const XmlDocument& /*doc*/) { return true;}

		virtual bool VisitExit(const XmlDocument& /*doc*/) {	return true;}

		virtual bool VisitEnter(const XmlElement& /*element*/, const XmlAttribute* /*firstAttribute*/) {	return true;}

		/* Visit an element. */
		virtual bool VisitExit(const XmlElement& /*element*/){ return true;}

		virtual bool Visit(const XmlDeclaration& /*declaration*/){ return true;}

		virtual bool Visit(const XmlText& /*text*/){ return true;}

		virtual bool Visit(const XmlComment& /*comment*/){ return true;}

		virtual bool Visit(const XmlUnknown& /*unknown*/){ return true;}
};

/* Only used by Attribute::Query functions */
enum {
	XML_SUCCESS,
	XML_NO_ATTRIBUTE,
	XML_WRONG_TYPE
};

/* Used by the parsing routines. */
enum XmlEncoding {
	XML_ENCODING_UNKNOWN,
	XML_ENCODING_UTF8,
	XML_ENCODING_LEGACY
};

const XmlEncoding TIXML_DEFAULT_ENCODING = XML_ENCODING_UNKNOWN;

/* XmlBase is a base class for every class in TinyXml.
 *	It does little except to establish that TinyXml classes	can be printed
 *	and provide some utility functions.
 *	In XML, the document and elements can contain	other elements and other types of nodes.
 *	A Document can contain:
														 Element	(container or leaf)
														 Comment (leaf)
														 Unknown (leaf)
														 Declaration( leaf )
 *	An Element can contain:
														 Element (container or leaf)
														 Text	(leaf)
														 Attributes (not on tree)
														 Comment (leaf)
														 Unknown (leaf)
 *	A Decleration contains: Attributes (not on tree) */
class XmlBase {
		friend class XmlNode;
		friend class XmlElement;
		friend class XmlDocument;

	public:
		XmlBase() :	userData(0){
			// do nothing.
		}

		virtual ~XmlBase(){
			// do nothing.
		}

		/* All TinyXml classes can print themselves to a filestream or the string class
		 * (TiXmlString in non-STL mode, std::string in STL mode.) Either or both cfile and str can be null.
		 * This is a formatted print, and will insert tabs and newlines.
		 * (For an unformatted stream, use the << operator.) */
		virtual void Print(FILE* cfile, int depth) const = 0;

		/* The world does not agree on whether white space should be kept or	not.
		 * In order to make everyone happy, these global, static functions are provided
		 * to set whether or not TinyXml will condense all white space into a single space or not.
		 * The default is to condense. Note changing this	value is not thread safe.	*/
		static void SetCondenseWhiteSpace(bool condense){
			condenseWhiteSpace = condense;
		}

		/* Return the current white space setting. */
		static bool IsWhiteSpaceCondensed(){
			return condenseWhiteSpace;
		}

		/* Return the position, in the original source file, of this node or attribute.
		 * The row and column are 1-based. (That is the first row and first column is	1,1).
		 * If the returns values are 0 or less, then the parser does not have a row and column value.
		 * Generally, the row and column value will be set when the XmlDocument::Load(),
		 * XmlDocument::LoadFile(), or any XmlNode::Parse() is called.
		 * It will NOT be set when the DOM was created from operator>>.
		 * The values reflect the initial load. Once the DOM is modified programmatically
		 * (by adding or changing nodes and attributes) the new values will NOT update to reflect changes in the document.
		 * There is a minor performance cost to computing the row and column.
		 * Computation	can be disabled if XmlDocument::SetTabSize() is called with 0 as the value.
		 * @sa XmlDocument::SetTabSize() */
		int Row() const {
			return location.row + 1;
		}

		int Column() const{ // See Row()
			return location.col + 1;
		}

		/* Set a pointer to arbitrary user data. */
		void SetUserData(void* user){
			userData = user;
		}

		/* Get a pointer to arbitrary user data. */
		void* GetUserData(){
			return userData;
		}

		/* Get a pointer to arbitrary user data. */
		const void* GetUserData() const {
			return userData;
		}

		/* Table that returs, for a given lead byte, the total number of bytes in the UTF-8 sequence. */
		static const int utf8ByteTable[256];

		virtual const YFCHAR * Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding/*= XML_ENCODING_UNKNOWN*/) = 0;

		/* Expands entities in a string. Note this should not contian the tag's '<', '>', etc,
		 * or they will be transformed into entities! */
		static void EncodeString(const STRING& str, STRING* out);

		enum {
			TIXML_NO_ERROR = 0,
			TIXML_ERROR,
			TIXML_ERROR_OPENING_FILE,
			TIXML_ERROR_PARSING_ELEMENT,
			TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
			TIXML_ERROR_READING_ELEMENT_VALUE,
			TIXML_ERROR_READING_ATTRIBUTES,
			TIXML_ERROR_PARSING_EMPTY,
			TIXML_ERROR_READING_END_TAG,
			TIXML_ERROR_PARSING_UNKNOWN,
			TIXML_ERROR_PARSING_COMMENT,
			TIXML_ERROR_PARSING_DECLARATION,
			TIXML_ERROR_DOCUMENT_EMPTY,
			TIXML_ERROR_EMBEDDED_NULL,
			TIXML_ERROR_PARSING_CDATA,
			TIXML_ERROR_DOCUMENT_TOP_ONLY,

			TIXML_ERROR_STRING_COUNT
		};

	protected:
		static const YFCHAR* SkipWhiteSpace(const YFCHAR*, XmlEncoding encoding);

		inline static bool IsWhiteSpace(YFCHAR c){
			return (isspace((YFCHAR) c) || c == '\n' || c == '\r');
		}

		inline static bool IsWhiteSpace(int c){ // only truly correct for English/Latin...but usually works.
			if (c < 256)
				return IsWhiteSpace((YFCHAR) c);
			return false;
		}

		/* Reads an XML name into the string provided.
		 * Returns a pointer just past the last character of the name, or 0 if the function has an error.*/
		static const YFCHAR* ReadName(const YFCHAR* p, STRING* name, XmlEncoding encoding);

		/* Reads text.
		 * Returns a pointer past the given end tag.
		 * Wickedly complex options, but it keeps the (sensitive) code in one place.*/
		static const YFCHAR* ReadText(const YFCHAR* in, // where to start
		                            STRING* text, // the string read
		                            bool ignoreWhiteSpace, // whether to keep the white space
		                            const YFCHAR* endTag, // what ends this text
		                            bool ignoreCase, // whether to ignore case in the end tag
		                            XmlEncoding encoding); // the current encoding

		/* If an entity has been found, transform it into a character. */
		static const YFCHAR* GetEntity(const YFCHAR* in, YFCHAR* value, int* length, XmlEncoding encoding);

		/* Get a character, while interpreting entities. The length can be from 0 to 4 bytes. */
		inline static const YFCHAR* GetChar(const YFCHAR* p, YFCHAR* _value, int* length, XmlEncoding encoding){
			assert(p);
			if (encoding == XML_ENCODING_UTF8){
				*length = utf8ByteTable[ *((const YFCHAR*) p)];
				assert( *length >= 0 && *length < 5);
			}else{
				*length = 1;
			}

			if ( *length == 1){
				if ( *p == '&')
					return GetEntity(p, _value, length, encoding);
				*_value = *p;
				return p + 1;
			}else if ( *length){
				for (int i = 0; p[i] && i < *length; ++i){
					_value[i] = p[i]; //strncpy(_value, p, *length)
				}
				return p + ( *length);
			}else{ // Not valid text.
				return 0;
			}
		}

		/* Return true if the next characters in the stream are any of the endTag sequences.
		 * Ignore case only works for english, and should only be relied on when comparing
		 * to English words: StringEqual( p, "version", true ) is fine. */
		static bool StringEqual(const YFCHAR* p, const YFCHAR* endTag, bool ignoreCase, XmlEncoding encoding);
		static const YFCHAR* errorString[TIXML_ERROR_STRING_COUNT];
		XmlCursor location;
		void* userData; // Field containing a generic user pointer

		/* None of these methods are reliable for any language except English.
		 * Good for approximation, not great for accuracy.*/
		static int IsAlpha(YFCHAR anyByte, XmlEncoding encoding);
		static int IsAlphaNum(YFCHAR anyByte, XmlEncoding encoding);
		inline static int ToLower(int v, XmlEncoding encoding){
			if (encoding == XML_ENCODING_UTF8){
				if (v < 128)
					return tolower(v);
				return v;
			}else{
				return tolower(v);
			}
		}

		static void ConvertUTF32ToUTF8(unsigned long input, YFCHAR* output, int* length);

	private:
		XmlBase(const XmlBase&); // not implemented.
		void operator=(const XmlBase& base); // not allowed.

		struct Entity {
				const YFCHAR* str;
				unsigned int strLength;
				YFCHAR chr;
		};
		enum {
			NUM_ENTITY = 5,
			MAX_ENTITY_LENGTH = 6
		};
		static Entity entity[NUM_ENTITY];
		static bool condenseWhiteSpace;
};

/* The parent class for everything in the Document Object Model.(Except for attributes).
 * Nodes have siblings, a parent, and children. A node can be in a document, or stand on its own.
 * The type of a XmlNode can be queried, and it can be cast to its more defined type. */
class XmlNode : public TreeNode<XmlNode, YFCHAR>, public XmlBase{
		friend class XmlDocument;
		friend class XmlElement;

	public:
		/* The types of XML nodes supported by TinyXml.
		 * (All the unsupported types are picked up by UNKNOWN.) */
		enum NodeType {
			TINYXML_DOCUMENT,
			TINYXML_ELEMENT,
			TINYXML_COMMENT,
			TINYXML_UNKNOWN,
			TINYXML_TEXT,
			TINYXML_DECLARATION,
			TINYXML_TYPECOUNT
		};

		virtual ~XmlNode();

		/* The meaning of 'value' changes for the specific type of	XmlNode.The subclasses will wrap this function.
		 * Document:	filename of the xml file
		 * Element:	name of the element
		 * Comment:	the comment text
		 * Unknown:	the tag contents
		 * Text:		  the text string	*/
		const YFCHAR *Value() const {
			return value.c_str( );
		}

		const STRING& ValueTStr() const {
			return value;
		}

		void SetValue(const YFCHAR * _value){
			value = _value;
		}

		/* Convenience function to get through elements.
		 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
		 * Returns 0 if there is not another element.*/
		const XmlElement* NextSiblingElement() const;
		XmlElement* NextSiblingElement(){
			return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->NextSiblingElement( ));
		}

		/* Convenience function to get through elements.
		 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
		 * Returns 0 if there is not another element.	*/
		const XmlElement* NextSiblingElement(const YFCHAR *) const;
		XmlElement* NextSiblingElement(const YFCHAR *_next){
			return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->NextSiblingElement(_next));
		}

		/* Convenience function to get through elements. */
		const XmlElement* FirstChildElement() const;
		XmlElement* FirstChildElement(){
			return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->FirstChildElement( ));
		}

		/* Convenience function to get through elements. */
		const XmlElement* FirstChildElement(const YFCHAR * _value) const;
		XmlElement* FirstChildElement(const YFCHAR * _value){
			return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->FirstChildElement(_value));
		}

		/* Query the type (as an enumerated value, above) of this node.
		 * The possible types are: DOCUMENT, ELEMENT, COMMENT,	UNKNOWN, TEXT, and DECLARATION.*/
		int Type() const { return type;}

		/* Return a pointer to the Document this node lives in.
		 * Returns null if not in a document.	*/
		const XmlDocument* GetDocument() const;
		XmlDocument* GetDocument(){
			return const_cast < XmlDocument* > ((const_cast < const XmlNode* > (this))->GetDocument( ));
		}

		/* Returns true if this node has no children. */
		bool NoChildren() const{
			return !firstChild;
		}

		virtual const XmlDocument* ToDocument() const {	return 0;}
		virtual const XmlElement* ToElement() const { return 0;}
		virtual const XmlComment* ToComment() const {	return 0;}
		virtual const XmlUnknown* ToUnknown() const {	return 0;}
		virtual const XmlText* ToText() const { return 0;}
		virtual const XmlDeclaration* ToDeclaration() const {	return 0;}
		virtual XmlDocument* ToDocument(){ return 0;}
		virtual XmlElement* ToElement(){ return 0;}
		virtual XmlComment* ToComment(){ return 0;}
		virtual XmlUnknown* ToUnknown(){ return 0;}
		virtual XmlText* ToText(){ return 0;}
		virtual XmlDeclaration* ToDeclaration(){ return 0;}

		/* Create an exact duplicate of this node and return it.
		 * The memory must be deleted by the caller. */
		virtual XmlNode* Clone() const = 0;

		/* Accept a hierchical visit the nodes in the TinyXML DOM.
		 * Every node in the XML tree will be conditionally visited
		 * and the host will be called back	via the XmlVisitor interface.
		 * An example of using Accept():
						 XmlPrinter printer;
						 tinyxmlDoc.Accept( &printer );
						 const YFCHAR* xmlcstr = printer.CStr();	*/
		virtual bool Accept(XmlVisitor* visitor) const = 0;

	protected:
		XmlNode(NodeType _type);

		/* Copy to the allocated object.
		 * Shared functionality between Clone, Copy constructor, and the assignment operator.*/
		void CopyTo(XmlNode* target) const;

		/* Figure out what is at *p, and parse it.
		 * Returns null if it is not an xml node. */
		XmlNode* Identify(const YFCHAR* start, XmlEncoding encoding);

		NodeType type;
		STRING   value;
	private:
		XmlNode(const XmlNode&); // not implemented.
		void operator=(const XmlNode& base); // not allowed.
};

/* An attribute is a name-value pair.
 * Elements have an arbitrary	number of attributes, each with a unique name.
 * [NOTE] The attributes are not TiXmlNodes, since they are not part of the tinyXML document object model.
 * There are other suggested ways to look at this problem.*/
class XmlAttribute : public XmlBase {
		friend class XmlAttributeSet;

	public:
		XmlAttribute() : XmlBase( ){
			document = 0;
			prev = next = 0;
		}

		XmlAttribute(const YFCHAR * _name, const YFCHAR * _value){
			name = _name;
			value = _value;
			document = 0;
			prev = next = 0;
		}

		const YFCHAR* Name() const{
			return name.c_str( );
		}

		const YFCHAR* Value() const{
			return value.c_str( );
		}

		/* Return the value of this attribute, converted to an integer. */
		int IntValue() const;

		/* Return the value of this attribute, converted to a double. */
		double DoubleValue() const;

		/* Get the tinyxml string representation */
		const STRING& NameTStr() const{
			return name;
		}

		/* QueryIntValue examines the value string.
		 * It is an alternative to the IntValue() method with richer error checking.
		 *	If the value is an integer, it is stored in 'value' and the call returns XML_SUCCESS.
		 *	If it is not an integer, it returns XML_WRONG_TYPE.
		 *	A specialized but useful call.
		 *	[NOTE] for success it returns 0, which is the opposite of almost all other TinyXml calls.*/
		int QueryIntValue(int* _value) const;

		int QueryDoubleValue(double* _value) const;

		/* Set the name of this attribute. */
		void SetName(const YFCHAR* _name){
			name = _name;
		}

		/* Set the value. */
		void SetValue(const YFCHAR* _value){
			value = _value;
		}

		/* Set the value from an integer. */
		void SetIntValue(int _value);

		/* Set the value from a double. */
		void SetDoubleValue(double _value);

		/* Get the next sibling attribute in the DOM.
		 * Returns null at end. */
		const XmlAttribute* Next() const;
		XmlAttribute* Next(){
			return const_cast < XmlAttribute* > ((const_cast < const XmlAttribute* > (this))->Next( ));
		}

		/* Get the previous sibling attribute in the DOM.
		 * Returns null at beginning. */
		const XmlAttribute* Previous() const;
		XmlAttribute* Previous(){
			return const_cast < XmlAttribute* > ((const_cast < const XmlAttribute* > (this))->Previous( ));
		}

		bool operator==(const XmlAttribute& rhs) const {
			return rhs.name == name;
		}

		bool operator<(const XmlAttribute& rhs) const {
			return name < rhs.name;
		}

		bool operator>(const XmlAttribute& rhs) const {
			return name > rhs.name;
		}

		/* Attribute parsing starts: first letter of the name
		 * returns: the next YFCHAR after the value end quote*/
		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding);

		/* Prints this Attribute to a FILE stream. */
		virtual void Print(FILE* cfile, int depth) const {
			Print(cfile, depth, 0);
		}
		void Print(FILE* cfile, int depth, STRING* str) const;

		/* Set the document pointer so the attribute can report errors. */
		void SetDocument(XmlDocument* doc){
			document = doc;
		}

	private:
		XmlAttribute(const XmlAttribute&); // not implemented.
		void operator=(const XmlAttribute& base); // not allowed.

		XmlDocument* 	document; // A pointer back to a document, for error reporting.
		STRING 		name;
		STRING 		value;
		XmlAttribute* prev;
		XmlAttribute* next;
};

/* A class used to manage a group of attributes.
 * It is only used internally, both by the ELEMENT and the DECLARATION.
 * The set can be changed transparent to the Element and Declaration classes that use it,
 * but NOT transparent to the Attribute which has to implement a next() and previous() method.
 * Which makes it a bit problematic and prevents the use of STL.
 * This version is implemented with circular lists because:
 * 	- I like circular lists
 * 	- it demonstrates some independence from the (typical) doubly linked list.*/
class XmlAttributeSet {
	public:
		XmlAttributeSet();

		~XmlAttributeSet();

		void Add(XmlAttribute* attribute);

		void Remove(XmlAttribute* attribute);

		const XmlAttribute* First() const{
			return (sentinel.next == &sentinel) ? 0 : sentinel.next;
		}

		XmlAttribute* First(){
			return (sentinel.next == &sentinel) ? 0 : sentinel.next;
		}

		const XmlAttribute* Last() const{
			return (sentinel.prev == &sentinel) ? 0 : sentinel.prev;
		}

		XmlAttribute* Last(){
			return (sentinel.prev == &sentinel) ? 0 : sentinel.prev;
		}

		XmlAttribute* Find(const YFCHAR* _name) const;

		XmlAttribute* FindOrCreate(const YFCHAR* _name);

	private:
		/* Because of hidden/disabled copy-construktor in XmlAttribute (sentinel-element),
		 * this class must be also use a hidden/disabled copy-constructor. */
		XmlAttributeSet(const XmlAttributeSet&); // not allowed
		void operator=(const XmlAttributeSet&); // not allowed (as XmlAttribute)

		XmlAttribute sentinel;
};

/* The element is a container class.
 * It has a value, the element name, and can contain other elements, text, comments, and unknowns.
 * Elements also contain an arbitrary number of attributes.*/
class XmlElement : public XmlNode {
	public:
		XmlElement(const YFCHAR * in_value);

		XmlElement(const XmlElement&);

		void operator=(const XmlElement& base);

		virtual ~XmlElement();

		/* Given an attribute name, Attribute() returns the value for the attribute of that name,
		 * or null if none exists.*/
		const YFCHAR* Attribute(const YFCHAR* name) const;

		/* Given an attribute name, Attribute() returns the value for the attribute of that name,
		 * or null if none exists.
		 * If the attribute exists and can be converted to an integer,
		 * the integer value will be put in the return 'i', if 'i' is non-null.	*/
		const YFCHAR* Attribute(const YFCHAR* name, int* i) const;

		/* Given an attribute name, Attribute() returns the value for the attribute of that name,
		 * or null if none exists.
		 * If the attribute exists and can be converted to an double,
		 * the double value will be put in the return 'd', if 'd'	is non-null.*/
		const YFCHAR* Attribute(const YFCHAR* name, double* d) const;

		/* QueryIntAttribute examines the attribute
		 * -- it is an alternative to the Attribute() method with richer error checking.
		 * If the attribute is an integer, it is stored in 'value' and the call returns XML_SUCCESS.
		 * If it is not an integer, it returns XML_WRONG_TYPE.
		 * If the attribute does not exist, then XML_NO_ATTRIBUTE is returned.*/
		int QueryIntAttribute(const YFCHAR* name, int* _value) const;

		int QueryDoubleAttribute(const YFCHAR* name, double* _value) const;

		int QueryFloatAttribute(const YFCHAR* name, float* _value) const{
			double d;
			int result = QueryDoubleAttribute(name, &d);
			if (result == XML_SUCCESS){
				*_value = (float) d;
			}
			return result;
		}

		/* Sets an attribute of name to a given value.
		 * The attribute will be created if it does not exist, or changed if it does.*/
		void SetAttribute(const YFCHAR* name, const YFCHAR * _value);

		/* Sets an attribute of name to a given value.
		 * The attribute will be created if it does not exist, or changed if it does.*/
		void SetAttribute(const YFCHAR * name, int value);

		/* Sets an attribute of name to a given value.
		 * The attribute will be created if it does not exist, or changed if it does.*/
		void SetDoubleAttribute(const YFCHAR * name, double value);

		/* Deletes an attribute with the given name. */
		void RemoveAttribute(const YFCHAR * name);

		/* Access the first attribute in this element. */
		const XmlAttribute* FirstAttribute() const{
			return attributeSet.First( );
		}

		XmlAttribute* FirstAttribute(){
			return attributeSet.First( );
		}

		/* Access the last attribute in this element. */
		const XmlAttribute* LastAttribute() const{
			return attributeSet.Last( );
		}

		XmlAttribute* LastAttribute(){
			return attributeSet.Last( );
		}


		/* Convenience function for easy access to the text inside an element.
		 * Although easy and concise, GetText() is limited compared to getting the XmlText child and accessing it directly.
		 * If the first child of 'this' is a XmlText, the GetText() returns the character string of the Text node, else null is returned.
		 * This is a convenient method for getting the text of simple contained text:
		 	 	 	 	 <foo>This is text</foo>
		 const YFCHAR* str = fooElement->GetText();
		 * 'str' will be a pointer to "This is text".
		 * Note that this function can be misleading. If the element foo was created from this XML:
		 	 	 	 	 <foo><b>This is text</b></foo>
		 * then the value of str would be null. The first child node isn't a text node, it is another element. From this XML:
		 	 	 	 	 <foo>This is <b>text</b></foo>
		 * GetText() will return "This is ".
		 * [NOTE]  GetText() accesses a child node
		 * -- don't become confused with the similarly named XmlHandle::Text() and XmlNode::ToText()
		 * which are safe type casts on the referenced node.*/
		const YFCHAR* GetText() const;

		/* Creates a new Element and returns it - the returned element is a copy. */
		virtual XmlNode* Clone() const;

		/* Print the Element to a FILE stream. */
		virtual void Print(FILE* cfile, int depth) const;

		/* Attribtue parsing starts: next YFCHAR past '<'
		 * returns: next YFCHAR past '>'	*/
		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding);

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual const XmlElement* ToElement() const{
			return this;
		}

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual XmlElement* ToElement(){
			return this;
		}

		/* Walk the XML tree visiting this node and all of its children. */
		virtual bool Accept(XmlVisitor* visitor) const;

	protected:
		void CopyTo(XmlElement* target) const;

		void ClearThis(); // like clear, but initializes 'this' object as well.

		/* Reads the "value" of the element, another element, or text.
		 * This should terminate with the current end tag.*/
		const YFCHAR* ReadValue(const YFCHAR* in, XmlParsingData* prevData, XmlEncoding encoding);

	private:
		XmlAttributeSet attributeSet;
};

/* An XML comment */
class XmlComment : public XmlNode {
	public:
		XmlComment() :
			XmlNode(XmlNode::TINYXML_COMMENT){
			// Constructs an empty comment.
		}

		/* Construct a comment from text. */
		XmlComment(const YFCHAR* _value) :
			XmlNode(XmlNode::TINYXML_COMMENT){
			SetValue(_value);
		}

		XmlComment(const XmlComment&);

		void operator=(const XmlComment& base);

		virtual ~XmlComment(){
			// do nothing.
		}

		/* Returns a copy of this Comment. */
		virtual XmlNode* Clone() const;

		/* Write this Comment to a FILE stream. */
		virtual void Print(FILE* cfile, int depth) const;

		/* Attribtue parsing starts: at the ! of the !--
		 * returns: next YFCHAR past '>'	*/
		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding);

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual const XmlComment* ToComment() const {
			return this;
		}

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual XmlComment* ToComment(){
			return this;
		}

		/* Walk the XML tree visiting this node and all of its children. */
		virtual bool Accept(XmlVisitor* visitor) const;

	protected:
		void CopyTo(XmlComment* target) const;
};

/* XML text.
 * A text node can have 2 ways to output the next: "normal" output and CDATA.
 * It will default to the mode it was parsed from the XML file and you generally want to leave it alone,
 * but you can change the output mode with SetCDATA() and query it with CDATA().*/
class XmlText : public XmlNode {
		friend class XmlElement;
	public:
		/* Constructor for text element.
		 * By default, it is treated as normal, encoded text.
		 * If you want it be output as a CDATA text	element, set the parameter _cdata to 'true'	*/
		XmlText(const YFCHAR * initValue) :
			XmlNode(XmlNode::TINYXML_TEXT){
			SetValue(initValue);
			cdata = false;
		}

		virtual ~XmlText(){
			// do nothing.
		}

		XmlText(const XmlText& copy) :
			XmlNode(XmlNode::TINYXML_TEXT){
			copy.CopyTo(this);
		}

		void operator=(const XmlText& base){
			base.CopyTo(this);
		}

		/* Write this text object to a FILE stream. */
		virtual void Print(FILE* cfile, int depth) const;

		/* Queries whether this represents text using a CDATA section. */
		bool CDATA() const{
			return cdata;
		}

		/* Turns on or off a CDATA representation of text. */
		void SetCDATA(bool _cdata){
			cdata = _cdata;
		}

		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding);

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual const XmlText* ToText() const {
			return this;
		}

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual XmlText* ToText(){
			return this;
		}

		/* Walk the XML tree visiting this node and all of its children. */
		virtual bool Accept(XmlVisitor* content) const;

	protected:
		/* Creates a new Element and returns it. */
		virtual XmlNode* Clone() const;

		void CopyTo(XmlText* target) const;

		/* returns true if all white space and new lines */
		bool Blank() const;

	private:
		bool cdata; // true if this should be input and output as a CDATA style text element
};

/* In correct XML the declaration is the first entry in the file.
 <?xml version="1.0" standalone="yes"?>
 * TinyXml will happily read or write files without a declaration,
 * however. There are 3 possible attributes to the declaration:version, encoding, and standalone.
 * [NOTE] In this version of the code, the attributes are handled as special cases,
 * not generic attributes, simply because there can only be at most 3 and they are always the same.*/
class XmlDeclaration : public XmlNode{
	public:
		XmlDeclaration() :
			XmlNode(XmlNode::TINYXML_DECLARATION){
			// do nothing.
		}

		XmlDeclaration(const YFCHAR* _version, const YFCHAR* _encoding, const YFCHAR* _standalone);

		XmlDeclaration(const XmlDeclaration& copy);

		void operator=(const XmlDeclaration& copy);

		virtual ~XmlDeclaration(){
			// do nothing.
		}

		/* Version.
		 * Will return an empty string if none was found. */
		const YFCHAR *Version() const{
			return version.c_str( );
		}

		/* Encoding.
		 * Will return an empty string if none was found. */
		const YFCHAR *Encoding() const{
			return encoding.c_str( );
		}

		/* Is this a standalone document */
		const YFCHAR *Standalone() const{
			return standalone.c_str( );
		}

		/* Creates a copy of this Declaration and returns it. */
		virtual XmlNode* Clone() const;

		/* Print this declaration to a FILE stream. */
		virtual void Print(FILE* cfile, int depth, STRING* str) const;

		virtual void Print(FILE* cfile, int depth) const{
			Print(cfile, depth, 0);
		}

		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding);

		/* Cast to a more defined type.
		 * Will return null not of the requested type. */
		virtual const XmlDeclaration* ToDeclaration() const{
			return this;
		}

		/* Cast to a more defined type.
		 * Will return null not of the requested type. */
		virtual XmlDeclaration* ToDeclaration(){
			return this;
		}

		/* Walk the XML tree visiting this node and all of its children. */
		virtual bool Accept(XmlVisitor* visitor) const;

	protected:
		void CopyTo(XmlDeclaration* target) const;

	private:
		STRING version;STRING encoding;STRING standalone;
};

/* Any tag that tinyXml doesn't recognize is saved as an unknown.
 * It is a tag of text, but should not be modified. It will be written back to the XML, unchanged, when the file is saved.
 * DTD tags get thrown into TiXmlUnknowns. */
class XmlUnknown : public XmlNode {
	public:
		XmlUnknown() : XmlNode(XmlNode::TINYXML_UNKNOWN){
			// do nothing.
		}

		virtual ~XmlUnknown(){
			// do nothing.
		}

		XmlUnknown(const XmlUnknown& copy) : XmlNode(XmlNode::TINYXML_UNKNOWN){
			copy.CopyTo(this);
		}

		void operator=(const XmlUnknown& copy){
			copy.CopyTo(this);
		}

		/* Creates a copy of this Unknown and returns it. */
		virtual XmlNode* Clone() const;

		/* Print this Unknown to a FILE stream.*/
		virtual void Print(FILE* cfile, int depth) const;

		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data, XmlEncoding encoding);

		/* Cast to a more defined type.
		 * Will return NULL not of the requested type. */
		virtual const XmlUnknown* ToUnknown() const{
			return this;
		}

		/* Cast to a more defined type.
		 * Will return NULL not of the requested type. */
		virtual XmlUnknown* ToUnknown(){
			return this;
		}

		/* Walk the XML tree visiting this node and all of its children. */
		virtual bool Accept(XmlVisitor* content) const;

	protected:
		void CopyTo(XmlUnknown* target) const;
};

/* A document binds together all the XML pieces. It can be saved, loaded, and printed to the screen.
 * Always the top level node. The 'value' of a document node is the xml file name.*/
class XmlDocument : public XmlNode {
	public:
		XmlDocument();

		/* Create a document with a name. The name of the document is also the filename of the xml. */
		XmlDocument(const YFCHAR * documentName);
		XmlDocument(const XmlDocument& copy);
		void operator=(const XmlDocument& copy);
		virtual ~XmlDocument(){
			// do nothing.
		}

		/* Load a file using the current document value.
		 * Returns true if successful. Will delete any existing document data before loading.	*/
		bool LoadFile(XmlEncoding encoding = TIXML_DEFAULT_ENCODING);

		/* Save a file using the current document value.
		 * Returns true if successful. */
		bool SaveFile() const;

		/* Load a file using the given filename.
		 * Returns true if successful. */
		bool LoadFile(const YFCHAR * filename, XmlEncoding encoding = TIXML_DEFAULT_ENCODING);

		/* Save a file using the given filename.
		 * Returns true if successful. */
		bool SaveFile(const YFCHAR * filename) const;

		/* Load a file using the given FILE*.
		 * Returns true if successful.
		 * [NOTE] this method doesn't stream:
		 * the entire object pointed at by the FILE*	will be interpreted as an XML file.
		 * TinyXML doesn't stream in XML from the current	file location. */
		bool LoadFile(FILE*, XmlEncoding encoding = TIXML_DEFAULT_ENCODING);

		/* Save a file using the given FILE*.
		 * Returns true if successful.*/
		bool SaveFile(FILE*) const;

		/* Parse the given null terminated block of xml data.
		 * Passing in an encoding to this	method (either XML_ENCODING_LEGACY or XML_ENCODING_UTF8) will force
		 * TinyXml to use that encoding, regardless of what TinyXml might otherwise try to detect.	*/
		virtual const YFCHAR* Parse(const YFCHAR* p, XmlParsingData* data = 0, XmlEncoding encoding = TIXML_DEFAULT_ENCODING);

		/* Get the root element -- the only top level element -- of the document.
		 * In well formed XML, there should only be one.
		 * TinyXml is tolerant of multiple elements at the document level.	*/
		const XmlElement* RootElement() const{
			return FirstChildElement( );
		}

		XmlElement* RootElement(){
			return FirstChildElement( );
		}

		/* If an error occurs, Error will be set to true.
		 * Also, The ErrorId() will contain the integer identifier of the error (not generally useful)
		 * The ErrorDesc() method will return the name of the error. (very useful)
		 * The ErrorRow() and ErrorCol() will return the location of the error (if known)	*/
		bool Error() const{
			return error;
		}

		/* Contains a textual (english) description of the error if one occurs. */
		const YFCHAR * ErrorDesc() const{
			return errorDesc.c_str( );
		}

		/* Generally, you probably want the error string ( ErrorDesc() ).
		 * But if you	prefer the ErrorId, this function will fetch it.*/
		int ErrorId() const{
			return errorId;
		}

		/* Returns the location (if known) of the error.
		 * The first column is column 1, and the first row is row 1.
		 * A value of 0 means the row and column wasn't applicable (memory errors, for example, have no row/column)
		 * or the parser lost the error. (An error in the error reporting, in that case.)	*/
		int ErrorRow() const{
			return errorLocation.row + 1;
		}

		/* The column where the error occured. See ErrorRow() */
		int ErrorCol() const{
			return errorLocation.col + 1;
		}

		/* SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol())
		 * to report the correct values for row and column.
		 * It does not change the output or input in any way.
		 * By calling this method, with a tab size	greater than 0, the row and column of each node and attribute is stored
		 * when the file is loaded. Very useful for tracking the DOM back in to the source file.
		 * The tab size is required for calculating the location of nodes.
		 * If not set, the default of 4 is used. The tabsize is set per document.
		 * Setting the tabsize to 0 disables row/column tracking.
		 * [Note]  row and column tracking is not supported when using operator>>.
		 * The tab size needs to be enabled before the parse or load. Correct usage:
		 XmlDocument doc;
		 doc.SetTabSize( 8 );
		 doc.Load( "myfile.xml" );	*/
		void SetTabSize(int _tabsize){
			tabsize = _tabsize;
		}

		int TabSize() const{
			return tabsize;
		}

		/* If you have handled the error, it can be reset with this call.
		 * The error state is automatically cleared if you Parse a new XML block.*/
		void ClearError(){
			error = false;
			errorId = 0;
			errorDesc = "";
			errorLocation.row = errorLocation.col = 0;
		}

		/* Write the document to standard out using formatted printing ("pretty print"). */
		void Print() const{
			Print(stdout, 0);
		}

		/* Print this Document to a FILE stream. */
		virtual void Print(FILE* cfile, int depth = 0) const;

		void SetError(int err, const YFCHAR* errorLocation, XmlParsingData* prevData,
		              XmlEncoding encoding);

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual const XmlDocument* ToDocument() const{
			return this;
		}

		/* Cast to a more defined type. Will return null not of the requested type. */
		virtual XmlDocument* ToDocument(){
			return this;
		}

		/* Walk the XML tree visiting this node and all of its children. */
		virtual bool Accept(XmlVisitor* content) const;

	protected:
		virtual XmlNode* Clone() const;

	private:
		void CopyTo(XmlDocument* target) const;

		bool error;
		int errorId;
		STRING errorDesc;
		int tabsize;
		XmlCursor errorLocation;
		bool useMicrosoftBOM; // the UTF-8 BOM were found when read. Note this, and try to write.
};

/* Print to memory functionality. The XmlPrinter is useful when you need to:
 * - Print to memory (especially in non-STL mode)
 * - Control formatting (line endings, etc.)
 * When constructed, the XmlPrinter is in its default "pretty printing" mode.
 * Before calling Accept() you can call methods to control the printing of the XML document.
 * After XmlNode::Accept() is called, the printed document can	be accessed via the CStr(), Str(), and Size() methods.
 * XmlPrinter uses the Visitor API:
				 XmlPrinter  printer;
				 printer.SetIndent( "\t" );
				 doc.Accept( &printer );
				 fprintf( stdout, "%s", printer.CStr() ); */
class XmlPrinter : public XmlVisitor {
	public:
		XmlPrinter() :	depth(0), simpleTextPrint(false), buffer( ), indent("  "), lineBreak("\n"){
			// do nothing.
		}

		virtual bool VisitEnter(const XmlDocument& doc);
		virtual bool VisitExit(const XmlDocument& doc);
		virtual bool VisitEnter(const XmlElement& element, const XmlAttribute* firstAttribute);
		virtual bool VisitExit(const XmlElement& element);
		virtual bool Visit(const XmlDeclaration& declaration);
		virtual bool Visit(const XmlText& text);
		virtual bool Visit(const XmlComment& comment);
		virtual bool Visit(const XmlUnknown& unknown);

		/* Set the indent characters for printing.
		 * By default 4 spaces but tab (\t) is also useful, or null/empty string for no indentation.*/
		void SetIndent(const YFCHAR* _indent){
			indent = _indent ? _indent : "";
		}

		/* Query the indention string. */
		const YFCHAR* Indent(){
			return indent.c_str( );
		}

		/* Set the line breaking string. By default set to newline (\n).
		 * Some operating systems prefer other characters,	or can be	set to the null/empty string for no indenation.	*/
		void SetLineBreak(const YFCHAR* _lineBreak){
			lineBreak = _lineBreak ? _lineBreak : "";
		}

		/* Query the current line breaking string. */
		const YFCHAR* LineBreak(){
			return lineBreak.c_str( );
		}

		/* Switch over to "stream printing" which is the most dense formatting without	linebreaks.
		 * Common when the XML is needed for network transmission.	*/
		void SetStreamPrinting(){
			indent = "";
			lineBreak = "";
		}

		/* Return the result. */
		const YFCHAR* CStr(){
			return buffer.c_str( );
		}

		/* Return the length of the result string. */
		size_t Size(){
			return buffer.size( );
		}

 private:
		void DoIndent(){
			for (int i = 0; i < depth; ++i)
				buffer += indent;
		}

		void DoLineBreak(){
			buffer += lineBreak;
		}

		int depth;
		bool simpleTextPrint;STRING buffer;STRING indent;STRING lineBreak;
};

/* XmlOutStream is an emulation of std::ostream.
 * It is based on String. Only the operators that we need for TinyXML have been developped.*/
class XmlOutStream : public String {
	public:
		XmlOutStream & operator <<(const String & in){
			*this += in;
			return *this;
		}

		XmlOutStream & operator <<(const YFCHAR * in){
			*this += in;
			return *this;
		}
};
} /// namespace YF

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
