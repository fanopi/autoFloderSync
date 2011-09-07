/*
 * xml.cc
 *
 *  Created on: 2011-5-4
 *      Author: 代码基于 TinyXML项目 重构 原作者Lee Thomason
 */

#include <ctype.h>

#include "xml.h"

namespace YF{

	const String::SIZE String::npos = static_cast < String::SIZE > ( -1);

	String::Rep String::nullrep_ = {0, 0, {'\0'}};// Null rep.

FILE* XmlFOpen(const YFCHAR* filename, const YFCHAR* mode);

bool XmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* XmlFOpen(const YFCHAR* filename, const YFCHAR* mode){
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
	FILE* fp = 0;
	errno_t err = fopen_s( &fp, filename, mode );
	if ( !err && fp )
		return fp;
	return 0;
#else
	return fopen(filename, mode);
#endif
}

void XmlBase::EncodeString(const STRING& str, STRING* outString){
	int i = 0;

	while (i < (int) str.length( )){
		YFCHAR c = (YFCHAR) str[i];
		if (c == '&' && i < ((int) str.length( ) - 2) && str[i + 1] == '#' && str[i + 2] == 'x'){
			while (i < (int) str.length( ) - 1){ // fix the bug that if there is no ";"
				outString->append(str.c_str( ) + i, 1);
				++i;
				if (str[i] == ';')
					break;
			}
		}else if (c == '&'){
			outString->append(entity[0].str, entity[0].strLength);
			++i;
		}else if (c == '<'){
			outString->append(entity[1].str, entity[1].strLength);
			++i;
		}else if (c == '>'){
			outString->append(entity[2].str, entity[2].strLength);
			++i;
		}else if (c == '\"'){
			outString->append(entity[3].str, entity[3].strLength);
			++i;
		}else if (c == '\''){
			outString->append(entity[4].str, entity[4].strLength);
			++i;
		}else if (c < 32){
			// Easy pass at non-alpha/numeric/symbol Below 32 is symbolic.
			YFCHAR buf[32];


#if defined(TIXML_SNPRINTF)
			TIXML_SNPRINTF(buf, sizeof(buf), "&#x%02X;", (unsigned) (c & 0xff));
#else
			sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
#endif
			
			outString->append(buf, (int) strlen(buf));
			++i;
		}else{
			*outString += (YFCHAR) c;
			++i;
		}
	}/// while
}

//////////////////////  class XmlNode   ////////////////////////////////////////
XmlNode::XmlNode(NodeType _type) : TreeNode<XmlNode,YFCHAR>(0),	XmlBase( ){
	parent = NULL;
	type = _type;
	firstChild = NULL;
	lastChild = NULL;
	prev = NULL;
	next = NULL;
}

XmlNode::~XmlNode(){
	XmlNode* node = firstChild;
	XmlNode* temp = NULL;

	while (node){
		temp = node;
		node = node->next;
		delete temp;
	}
}

void XmlNode::CopyTo(XmlNode* target) const{
	target->SetValue(value.c_str( ));
	target->userData = userData;
	target->location = location;
}

const XmlElement* XmlNode::FirstChildElement() const {
	const XmlNode* node;
	for (node = FirstChild(); node; node = node->NextSibling()){
		if (node->ToElement( ))
			return node->ToElement( );
	}
	return NULL;
}

const XmlElement* XmlNode::FirstChildElement(const YFCHAR * _value) const{
	const XmlNode* node;
	for (node = FirstChild(_value); node; node = node->NextSibling(_value)){
		if (node->ToElement( ))
			return node->ToElement( );
	}
	return NULL;
}

const XmlElement* XmlNode::NextSiblingElement() const{
	for (const XmlNode* node = NextSibling( ); node; node = node->NextSibling( )){
		if (node->ToElement( ))
			return node->ToElement( );
	}
	return NULL;
}

const XmlElement* XmlNode::NextSiblingElement(const YFCHAR * _value) const{
	for (const XmlNode* node = NextSibling(_value); node; node = node->NextSibling(_value)){
		if (node->ToElement( ))
			return node->ToElement( );
	}
	return NULL;
}

const XmlDocument* XmlNode::GetDocument() const{
	for (const XmlNode* node = this; node; node = node->parent){
		if (node->ToDocument( ))
			return node->ToDocument( );
	}
	return NULL;
}

//////////////////////////////  calss XmlElement   /////////////////////////////
XmlElement::XmlElement(const YFCHAR * _value) :
	XmlNode(XmlNode::TINYXML_ELEMENT){
	firstChild = lastChild = NULL;
	value = _value;
}

XmlElement::XmlElement(const XmlElement& copy) :
	XmlNode(XmlNode::TINYXML_ELEMENT){
	firstChild = lastChild = NULL;
	copy.CopyTo(this);
}

void XmlElement::operator=(const XmlElement& base){
	ClearThis( );
	base.CopyTo(this);
}

XmlElement::~XmlElement(){
	ClearThis( );
}

void XmlElement::ClearThis(){
	Clear( );
	while (attributeSet.First( )){
		XmlAttribute* node = attributeSet.First( );
		attributeSet.Remove(node);
		delete node;
	}
}

const YFCHAR* XmlElement::Attribute(const YFCHAR* name) const{
	const XmlAttribute* node = attributeSet.Find(name);
	if (node)
		return node->Value( );
	return NULL;
}

const YFCHAR* XmlElement::Attribute(const YFCHAR* name, int* i) const {
	const XmlAttribute* attrib = attributeSet.Find(name);
	const YFCHAR* result = NULL;

	if (attrib){
		result = attrib->Value( );
		if (i)
			attrib->QueryIntValue(i);
	}
	return result;
}

const YFCHAR* XmlElement::Attribute(const YFCHAR* name, double* d) const {
	const XmlAttribute* attrib = attributeSet.Find(name);
	const YFCHAR* result = 0;

	if (attrib){
		result = attrib->Value( );
		if (d)
			attrib->QueryDoubleValue(d);
	}
	return result;
}

int XmlElement::QueryIntAttribute(const YFCHAR* name, int* ival) const {
	const XmlAttribute* attrib = attributeSet.Find(name);
	if ( !attrib)
		return XML_NO_ATTRIBUTE;
	return attrib->QueryIntValue(ival);
}

int XmlElement::QueryDoubleAttribute(const YFCHAR* name, double* dval) const {
	const XmlAttribute* attrib = attributeSet.Find(name);
	if ( !attrib)
		return XML_NO_ATTRIBUTE;
	return attrib->QueryDoubleValue(dval);
}

void XmlElement::SetAttribute(const YFCHAR * name, int val){
	XmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib){
		attrib->SetIntValue(val);
	}
}

void XmlElement::SetDoubleAttribute(const YFCHAR * name, double val){
	XmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib)
		attrib->SetDoubleValue(val);
}

void XmlElement::SetAttribute(const YFCHAR * cname, const YFCHAR * cvalue){
	XmlAttribute* attrib = attributeSet.FindOrCreate(cname);
	if (attrib)
		attrib->SetValue(cvalue);
}

void XmlElement::Print(FILE* cfile, int depth) const {
	int i;
	assert(cfile);
	for (i = 0; i < depth; i++){
		fprintf(cfile, "    ");
	}
	fprintf(cfile, "<%s", value.c_str( ));

	const XmlAttribute* attrib;
	for (attrib = attributeSet.First( ); attrib; attrib = attrib->Next( )){
		fprintf(cfile, " ");
		attrib->Print(cfile, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	XmlNode* node;
	if ( !firstChild){
		fprintf(cfile, " />");

	}else if (firstChild == lastChild && firstChild->ToText( )){
		fprintf(cfile, ">");
		firstChild->Print(cfile, depth + 1);
		fprintf(cfile, "</%s>", value.c_str( ));

	}else{
		fprintf(cfile, ">");
		for (node = firstChild; node; node = node->NextSibling( )){
			if ( !node->ToText( )) fprintf(cfile, "\n");
			node->Print(cfile, depth + 1);
		}
		fprintf(cfile, "\n");
		for (i = 0; i < depth; ++i){
			fprintf(cfile, "    ");
		}
		fprintf(cfile, "</%s>", value.c_str( ));
	}
}

void XmlElement::CopyTo(XmlElement* target) const {
	// superclass:
	XmlNode::CopyTo(target);
	// Element class:  Clone the attributes, then clone the children.
	const XmlAttribute* attribute = 0;
	for (attribute = attributeSet.First( ); attribute; attribute = attribute->Next( )){
		target->SetAttribute(attribute->Name( ), attribute->Value( ));
	}
	XmlNode* node = 0;
	for (node = firstChild; node; node = node->NextSibling( )){
		target->LinkEndChild(node->Clone( ));
	}
}

bool XmlElement::Accept(XmlVisitor* visitor) const {
	if (visitor->VisitEnter( *this, attributeSet.First( ))){
		for (const XmlNode* node = FirstChild( ); node; node = node->NextSibling( )){
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit( *this);
}

XmlNode* XmlElement::Clone() const {
	XmlElement* clone = new XmlElement(Value( ));
	if (!clone)
		return 0;
	CopyTo(clone);
	return clone;
}

const YFCHAR* XmlElement::GetText() const {
	const XmlNode* child = this->FirstChild( );
	if (child){
		const XmlText* childText = child->ToText( );
		if (childText)
			return childText->Value( );
	}
	return 0;
}

void XmlElement::RemoveAttribute(const YFCHAR * name){
	XmlAttribute* node = attributeSet.Find(name);
	if (node){
		attributeSet.Remove(node);
		delete node;
	}
}

/////////////////////////  calss XmlDocument   /////////////////////////////////
XmlDocument::XmlDocument() :
	XmlNode(XmlNode::TINYXML_DOCUMENT){
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError( );
}

XmlDocument::XmlDocument(const YFCHAR * documentName) :
	XmlNode(XmlNode::TINYXML_DOCUMENT){
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError( );
}

XmlDocument::XmlDocument(const XmlDocument& copy) :
	XmlNode(XmlNode::TINYXML_DOCUMENT){
	copy.CopyTo(this);
}

void XmlDocument::operator=(const XmlDocument& copy){
	Clear( );
	copy.CopyTo(this);
}

bool XmlDocument::LoadFile(XmlEncoding encoding){
	return LoadFile(Value( ), encoding);
}

bool XmlDocument::SaveFile() const {
	return SaveFile(Value( ));
}

bool XmlDocument::LoadFile(const YFCHAR* _filename, XmlEncoding encoding){
	STRING filename(_filename);
	value = filename;
	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = XmlFOpen(value.c_str( ), "rb");
	if (file){
		bool result = LoadFile(file, encoding);
		fclose(file);
		return result;
	}else{
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, XML_ENCODING_UNKNOWN);
		return false;
	}
}

bool XmlDocument::LoadFile(FILE* file, XmlEncoding encoding){
	if ( !file){
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, XML_ENCODING_UNKNOWN);
		return false;
	}
	// Delete the existing data:
	Clear( );
	location.Clear( );
	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Strange case, but good to handle up front.
	if (length <= 0){
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, XML_ENCODING_UNKNOWN);
		return false;
	}
	YFCHAR* buf = new YFCHAR[length + 1];
	buf[0] = 0;
	if (fread(buf, length, 1, file) != 1){
		delete[] buf;
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, XML_ENCODING_UNKNOWN);
		return false;
	}
	// Process the buffer in place to normalize new lines. (See comment above.)
	// Copies from the 'p' to 'q' pointer, where p can advance faster if
	// a newline-carriage return is hit.
	//
	// Wikipedia:
	// Systems based on ASCII or a compatible character set use either LF  (Line feed, '\n', 0x0A, 10 in decimal) or 
	// CR (Carriage return, '\r', 0x0D, 13 in decimal) individually, or CR followed by LF (CR+LF, 0x0D 0x0A)...
	//		* LF:    Multics, Unix and Unix-like systems (GNU/Linux, AIX, Xenix, Mac OS X, FreeBSD, etc.), BeOS, Amiga, RISC OS, and others
	//		* CR+LF: DEC RT-11 and most other early non-Unix, non-IBM OSes, CP/M, MP/M, DOS, OS/2, Microsoft Windows, Symbian OS
	//		* CR:    Commodore 8-bit machines, Apple II family, Mac OS up to version 9 and OS-9
	const YFCHAR* p = buf; // the read head
	YFCHAR* q = buf; // the write head
	const YFCHAR CR = 0x0d;
	const YFCHAR LF = 0x0a;
	buf[length] = 0;
	while ( *p){
		assert(p < (buf + length));
		assert(q <= (buf + length));
		assert(q <= p);
		if ( *p == CR){
			*q++ = LF;
			p++;
			if ( *p == LF) // check for CR+LF (and skip LF)
			p++;
		}else{
			*q++ = *p++;
		}
	}
	assert(q <= (buf + length));
	*q = 0;

	Parse(buf, 0, encoding);

	delete[] buf;
	return !Error( );
}

bool XmlDocument::SaveFile(const YFCHAR * filename) const {
	FILE* fp = XmlFOpen(filename, "w"); // The old c stuff lives on...
	if (fp){
		bool result = SaveFile(fp);
		fclose(fp);
		return result;
	}
	return false;
}

bool XmlDocument::SaveFile(FILE* fp) const {
	if (useMicrosoftBOM){
		const  YFCHAR TIXML_UTF_LEAD_0 = 0xefU;
		const  YFCHAR TIXML_UTF_LEAD_1 = 0xbbU;
		const  YFCHAR TIXML_UTF_LEAD_2 = 0xbfU;
		fputc(TIXML_UTF_LEAD_0, fp);
		fputc(TIXML_UTF_LEAD_1, fp);
		fputc(TIXML_UTF_LEAD_2, fp);
	}
	Print(fp, 0);
	return (ferror(fp) == 0);
}

void XmlDocument::CopyTo(XmlDocument* target) const {
	XmlNode::CopyTo(target);

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	XmlNode* node = NULL;
	for (node = firstChild; node; node = node->NextSibling( )){
		target->LinkEndChild(node->Clone( ));
	}
}

XmlNode* XmlDocument::Clone() const {
	XmlDocument* clone = new XmlDocument( );
	if ( !clone)
		return NULL;
	CopyTo(clone);
	return clone;
}

void XmlDocument::Print(FILE* cfile, int depth) const {
	assert(cfile);
	for (const XmlNode* node = FirstChild( ); node; node = node->NextSibling( )){
		node->Print(cfile, depth);
		fprintf(cfile, "\n");
	}
}

bool XmlDocument::Accept(XmlVisitor* visitor) const {
	if (visitor->VisitEnter( *this)){
		for (const XmlNode* node = FirstChild( ); node; node = node->NextSibling( )){
			if ( !node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit( *this);
}

//////////////////////////  calss XmlAttribute   ///////////////////////////////
const XmlAttribute* XmlAttribute::Next() const{
	if (next->value.empty( ) && next->name.empty( )) // We are using knowledge of the sentinel. The sentinel	have a value or name.
	return NULL;
	return next;
}

const XmlAttribute* XmlAttribute::Previous() const {
	if (prev->value.empty( ) && prev->name.empty( )) // We are using knowledge of the sentinel. The sentinel have a value or name.
	return NULL;
	return prev;
}

void XmlAttribute::Print(FILE* cfile, int /*depth*/, STRING* str) const{
	STRING n, v;
	EncodeString(name, &n);
	EncodeString(value, &v);

	if (value.find('\"') == STRING::npos){
		if (cfile) fprintf(cfile, "%s=\"%s\"", n.c_str( ), v.c_str( ));
		if (str){
			( *str) += n;
			( *str) += "=\"";
			( *str) += v;
			( *str) += "\"";
		}

	}else{
		if (cfile) fprintf(cfile, "%s='%s'", n.c_str( ), v.c_str( ));
		if (str){
			( *str) += n;
			( *str) += "='";
			( *str) += v;
			( *str) += "'";
		}
	}
}

int XmlAttribute::QueryIntValue(int* ival) const{
	if (TIXML_SSCANF(value.c_str( ), "%d", ival) == 1)
		return XML_SUCCESS;
	return XML_WRONG_TYPE;
}

int XmlAttribute::QueryDoubleValue(double* dval) const{
	if (TIXML_SSCANF(value.c_str( ), "%lf", dval) == 1)
		return XML_SUCCESS;
	return XML_WRONG_TYPE;
}

void XmlAttribute::SetIntValue(int _value){
	YFCHAR buf[64];
#if defined(TIXML_SNPRINTF)
	TIXML_SNPRINTF(buf, sizeof(buf), "%d", _value);
#else
	sprintf (buf, "%d", _value);
#endif
	SetValue(buf);
}

void XmlAttribute::SetDoubleValue(double _value){
	YFCHAR buf[256];
#if defined(TIXML_SNPRINTF)
	TIXML_SNPRINTF(buf, sizeof(buf), "%g", _value);
#else
	sprintf (buf, "%g", _value);
#endif
	SetValue(buf);
}

int XmlAttribute::IntValue() const {
	return atoi(value.c_str( ));
}

double XmlAttribute::DoubleValue() const{
	return atof(value.c_str( ));
}

///////////////////////  calss XmlComment   ////////////////////////////////////
XmlComment::XmlComment(const XmlComment& copy) :
	XmlNode(XmlNode::TINYXML_COMMENT){
	copy.CopyTo(this);
}

void XmlComment::operator=(const XmlComment& base){
	Clear( );
	base.CopyTo(this);
}

void XmlComment::Print(FILE* cfile, int depth) const{
	assert(cfile);
	for (int i = 0; i < depth; i++){
		fprintf(cfile, "    ");
	}
	fprintf(cfile, "<!--%s-->", value.c_str( ));
}

void XmlComment::CopyTo(XmlComment* target) const{
	XmlNode::CopyTo(target);
}

bool XmlComment::Accept(XmlVisitor* visitor) const{
	return visitor->Visit( *this);
}

XmlNode* XmlComment::Clone() const{
	XmlComment* clone = new XmlComment( );
	if ( !clone) return NULL;
	CopyTo(clone);
	return clone;
}

//////////////////////////  calss XmlText   ////////////////////////////////////
void XmlText::Print(FILE* cfile, int depth) const{
	assert(cfile);
	if (cdata){
		fprintf(cfile, "\n");
		for (int i = 0; i < depth; i++){
			fprintf(cfile, "    ");
		}
		fprintf(cfile, "<![CDATA[%s]]>\n", value.c_str( )); // unformatted output

	}else{
		STRING buffer;
		EncodeString(value, &buffer);
		fprintf(cfile, "%s", buffer.c_str( ));
	}
}

void XmlText::CopyTo(XmlText* target) const{
	XmlNode::CopyTo(target);
	target->cdata = cdata;
}

bool XmlText::Accept(XmlVisitor* visitor) const{
	return visitor->Visit( *this);
}

XmlNode* XmlText::Clone() const{
	XmlText* clone = new XmlText("");
	if ( !clone) return NULL;
	CopyTo(clone);
	return clone;
}

///////////////////////////  calss XmlDeclaration   ///////////////////////////
XmlDeclaration::XmlDeclaration(const YFCHAR * _version, const YFCHAR * _encoding, const YFCHAR * _standalone)
                          :	XmlNode(XmlNode::TINYXML_DECLARATION){
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}

XmlDeclaration::XmlDeclaration(const XmlDeclaration& copy)
                          :	XmlNode(XmlNode::TINYXML_DECLARATION){
	copy.CopyTo(this);
}

void XmlDeclaration::operator=(const XmlDeclaration& copy){
	Clear( );
	copy.CopyTo(this);
}

void XmlDeclaration::Print(FILE* cfile, int /*depth*/, STRING* str) const{
	if (cfile)
		fprintf(cfile, "<?xml ");
	if (str)
		( *str) += "<?xml ";
	if ( !version.empty( )){
		if (cfile)
			fprintf(cfile, "version=\"%s\" ", version.c_str( ));
		if (str){
			( *str) += "version=\"";
			( *str) += version;
			( *str) += "\" ";
		}
	}

	if ( !encoding.empty( )){
		if (cfile)
			fprintf(cfile, "encoding=\"%s\" ", encoding.c_str( ));
		if (str){
			( *str) += "encoding=\"";
			( *str) += encoding;
			( *str) += "\" ";
		}
	}

	if ( !standalone.empty( )){
		if (cfile)
			fprintf(cfile, "standalone=\"%s\" ", standalone.c_str( ));
		if (str){
			( *str) += "standalone=\"";
			( *str) += standalone;
			( *str) += "\" ";
		}
	}

	if (cfile)
		fprintf(cfile, "?>");
	if (str)
		( *str) += "?>";
}

void XmlDeclaration::CopyTo(XmlDeclaration* target) const{
	XmlNode::CopyTo(target);
	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}

bool XmlDeclaration::Accept(XmlVisitor* visitor) const{
	return visitor->Visit( *this);
}

XmlNode* XmlDeclaration::Clone() const{
	XmlDeclaration* clone = new XmlDeclaration( );
	if ( !clone) return NULL;
	CopyTo(clone);
	return clone;
}

////////////////////////  calss XmlUnknown   ///////////////////////////////////
void XmlUnknown::Print(FILE* cfile, int depth) const{
	for (int i = 0; i < depth; i++)
		fprintf(cfile, "    ");
	fprintf(cfile, "<%s>", value.c_str( ));
}

void XmlUnknown::CopyTo(XmlUnknown* target) const{
	XmlNode::CopyTo(target);
}

bool XmlUnknown::Accept(XmlVisitor* visitor) const{
	return visitor->Visit( *this);
}

XmlNode* XmlUnknown::Clone() const{
	XmlUnknown* clone = new XmlUnknown( );
	if ( !clone)
		return NULL;
	CopyTo(clone);
	return clone;
}

///////////////  calss XmlAttributeSet   ///////////////////////////////////////
XmlAttributeSet::XmlAttributeSet(){
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}

XmlAttributeSet::~XmlAttributeSet(){
	assert(sentinel.next == &sentinel);
	assert(sentinel.prev == &sentinel);
}

void XmlAttributeSet::Add(XmlAttribute* addMe){
	assert( !Find(addMe->Name( ))); // Shouldn't be multiply adding to the set.
	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;
	sentinel.prev->next = addMe;
	sentinel.prev = addMe;
}

void XmlAttributeSet::Remove(XmlAttribute* removeMe){
	for (XmlAttribute* node = sentinel.next; node != &sentinel; node = node->next){
		if (node == removeMe){
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = NULL;
			node->prev = NULL;
			return;
		}
	}
	assert(0); // we tried to remove a non-linked attribute.
}

XmlAttribute* XmlAttributeSet::Find(const YFCHAR* name) const {
	for (XmlAttribute* node = sentinel.next; node != &sentinel; node = node->next){
		if (strcmp(node->name.c_str( ), name) == 0) return node;
	}
	return NULL;
}

XmlAttribute* XmlAttributeSet::FindOrCreate(const YFCHAR* _name){
	XmlAttribute* attrib = Find(_name);
	if ( !attrib){
		attrib = new XmlAttribute( );
		Add(attrib);
		attrib->SetName(_name);
	}
	return attrib;
}


/////////////////  calss XmlPrinter   /////////////////////////////////////////
bool XmlPrinter::VisitEnter(const XmlDocument&){
	return true;
}

bool XmlPrinter::VisitExit(const XmlDocument&){
	return true;
}

bool XmlPrinter::VisitEnter(const XmlElement& element, const XmlAttribute* firstAttribute){
	DoIndent( );
	buffer += "<";
	buffer += element.Value( );
	for (const XmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next( )){
		buffer += " ";
		attrib->Print(0, 0, &buffer);
	}
	if ( !element.FirstChild( )){
		buffer += " />";
		DoLineBreak( );
	}else{
		buffer += ">";
		if (element.FirstChild( )->ToText( )
				&& element.LastChild( ) == element.FirstChild( )
		    && element.FirstChild( )->ToText( )->CDATA( ) == false){
			simpleTextPrint = true;
			// no DoLineBreak()!
		}else{
			DoLineBreak( );
		}
	}

	++depth;
	return true;
}

bool XmlPrinter::VisitExit(const XmlElement& element){
	--depth;
	if ( !element.FirstChild( )){
		// nothing.
	}else{
		if (simpleTextPrint)
			simpleTextPrint = false;
		else
			DoIndent( );
		buffer += "</";
		buffer += element.Value( );
		buffer += ">";
		DoLineBreak( );
	}
	return true;
}

bool XmlPrinter::Visit(const XmlText& text){
	if (text.CDATA( )){
		DoIndent( );
		buffer += "<![CDATA[";
		buffer += text.Value( );
		buffer += "]]>";
		DoLineBreak( );

	}else if (simpleTextPrint){
		STRING str;
		XmlBase::EncodeString(text.ValueTStr( ), &str);
		buffer += str;

	}else{
		DoIndent( );
		STRING str;
		XmlBase::EncodeString(text.ValueTStr( ), &str);
		buffer += str;
		DoLineBreak( );
	}
	return true;
}

bool XmlPrinter::Visit(const XmlDeclaration& declaration){
	DoIndent( );
	declaration.Print(0, 0, &buffer);
	DoLineBreak( );
	return true;
}

bool XmlPrinter::Visit(const XmlComment& comment){
	DoIndent( );
	buffer += "<!--";
	buffer += comment.Value( );
	buffer += "-->";
	DoLineBreak( );
	return true;
}

bool XmlPrinter::Visit(const XmlUnknown& unknown){
	DoIndent( );
	buffer += "<";
	buffer += unknown.Value( );
	buffer += ">";
	DoLineBreak( );
	return true;
}
} /// namespace YF
