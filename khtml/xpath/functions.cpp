/*
 * functions.cc - Copyright 2005 Frerich Raabe <raabe@kde.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "functions.h"

#include "xml/dom_nodeimpl.h"
#include "xml/dom_elementimpl.h"

#include <QtDebug>

#include <math.h>

using namespace DOM;

#define DEFINE_FUNCTION_CREATOR(Class) \
static Function *create##Class() { return new Class; }

class Interval
{
	public:
		static const int Inf =-1;

		Interval();
		Interval( int value );
		Interval( int min, int max );

		bool contains( int value ) const;

		QString asString() const;

	private:
		int m_min;
		int m_max;
};

class FunLast : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunPosition : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunCount : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunLocalName : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunNamespaceURI : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunName : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunString : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunConcat : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunStartsWith : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunContains : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunSubstringBefore : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunSubstringAfter : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunSubstring : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunStringLength : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunNormalizeSpace : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunTranslate : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunBoolean : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunNot : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunTrue : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunFalse : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunLang : public Function
{
	public:
		virtual bool isConstant() const;

	private:
		virtual Value doEvaluate() const;
};

class FunNumber : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunSum : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunFloor : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunCeiling : public Function
{
	private:
		virtual Value doEvaluate() const;
};

class FunRound : public Function
{
	private:
		virtual Value doEvaluate() const;
};

DEFINE_FUNCTION_CREATOR( FunLast )
DEFINE_FUNCTION_CREATOR( FunPosition )
DEFINE_FUNCTION_CREATOR( FunCount )
DEFINE_FUNCTION_CREATOR( FunLocalName )
DEFINE_FUNCTION_CREATOR( FunNamespaceURI )
DEFINE_FUNCTION_CREATOR( FunName )

DEFINE_FUNCTION_CREATOR( FunString )
DEFINE_FUNCTION_CREATOR( FunConcat )
DEFINE_FUNCTION_CREATOR( FunStartsWith )
DEFINE_FUNCTION_CREATOR( FunContains )
DEFINE_FUNCTION_CREATOR( FunSubstringBefore )
DEFINE_FUNCTION_CREATOR( FunSubstringAfter )
DEFINE_FUNCTION_CREATOR( FunSubstring )
DEFINE_FUNCTION_CREATOR( FunStringLength )
DEFINE_FUNCTION_CREATOR( FunNormalizeSpace )
DEFINE_FUNCTION_CREATOR( FunTranslate )

DEFINE_FUNCTION_CREATOR( FunBoolean )
DEFINE_FUNCTION_CREATOR( FunNot )
DEFINE_FUNCTION_CREATOR( FunTrue )
DEFINE_FUNCTION_CREATOR( FunFalse )
DEFINE_FUNCTION_CREATOR( FunLang )

DEFINE_FUNCTION_CREATOR( FunNumber )
DEFINE_FUNCTION_CREATOR( FunSum )
DEFINE_FUNCTION_CREATOR( FunFloor )
DEFINE_FUNCTION_CREATOR( FunCeiling )
DEFINE_FUNCTION_CREATOR( FunRound )

#undef DEFINE_FUNCTION_CREATOR

Interval::Interval()
	: m_min( Inf ),
	m_max( Inf )
{
}

Interval::Interval( int value )
	: m_min( value ),
	m_max( value )
{
}

Interval::Interval( int min, int max )
	: m_min( min ),
	m_max( max )
{
}

bool Interval::contains( int value ) const
{
	if ( m_min == Inf && m_max == Inf ) {
		return true;
	}

	if ( m_min == Inf ) {
		return value <= m_max;
	}

	if ( m_max == Inf ) {
		return value >= m_min;
	}

	return value >= m_min && value <= m_max;
}

QString Interval::asString() const
{
	QString s = "[";

	if ( m_min == Inf ) {
		s += "-Infinity";
	} else {
		s += QString::number( m_min );
	}

	s += "..";

	if ( m_max == Inf ) {
		s += "Infinity";
	} else {
		s += QString::number( m_max );
	}

	s += "]";

	return s;
}

void Function::setArguments( const QList<Expression *> &args )
{
	foreach( Expression *arg, args ) {
		addSubExpression( arg );
	}
}

void Function::setName( const QString &name )
{
	m_name = name;
}

QString Function::dump() const
{
	if ( argCount() == 0 ) {
		return QString( "<function name=\"%1\"/>" ).arg( name() );
	}

	QString s = QString( "<function name=\"%1\">" ).arg( name() );
	for ( unsigned int i = 0; i < argCount(); ++i ) {
		s += "<operand>" + arg( i )->dump() + "</operand>";
	}
	s += "</function>";
	return s;
}


Expression *Function::arg( int i )
{
	return subExpr( i );
}

const Expression *Function::arg( int i ) const
{
	return subExpr( i );
}

unsigned int Function::argCount() const
{
	return subExprCount();
}

QString Function::name() const
{
	return m_name;
}

Value FunLast::doEvaluate() const
{
	return Value( double( Expression::evaluationContext().size ) );
}

bool FunLast::isConstant() const
{
	return false;
}

Value FunPosition::doEvaluate() const
{
	return Value( double( Expression::evaluationContext().position ) );
}

bool FunPosition::isConstant() const
{
	return false;
}

bool FunLocalName::isConstant() const
{
	return false;
}

Value FunLocalName::doEvaluate() const
{
	NodeImpl *node = 0;
	if ( argCount() > 0 ) {
		Value a = arg( 0 )->evaluate();
		if ( a.isNodeset() ) {
			node = a.toNodeset().first();
		}
	}

	if ( !node ) {
		node = evaluationContext().node;
	}

	return Value( node->localName().string() );
}

bool FunNamespaceURI::isConstant() const
{
	return false;
}

Value FunNamespaceURI::doEvaluate() const
{
	NodeImpl *node = 0;
	if ( argCount() > 0 ) {
		Value a = arg( 0 )->evaluate();
		if ( a.isNodeset() ) {
			node = a.toNodeset().first();
		}
	}

	if ( !node ) {
		node = evaluationContext().node;
	}

	return Value( node->namespaceURI().string() );
}

bool FunName::isConstant() const
{
	return false;
}

Value FunName::doEvaluate() const
{
	NodeImpl *node = 0;
	if ( argCount() > 0 ) {
		Value a = arg( 0 )->evaluate();
		if ( a.isNodeset() ) {
			node = a.toNodeset().first();
		}
	}

	if ( !node ) {
		node = evaluationContext().node;
	}

	QString s = node->namespaceURI().string();
	s += ":";
	s += node->localName().string();

	return Value( s );
}

Value FunCount::doEvaluate() const
{
	Value a = arg( 0 )->evaluate();
	if ( !a.isNodeset() ) {
		qWarning( "count() expects <nodeset>" );
		return Value( 0.0 );
	}
	return Value( double( a.toNodeset().count() ) );
}

bool FunCount::isConstant() const
{
	return false;
}

Value FunString::doEvaluate() const
{
	if ( argCount() == 0 ) {
		DomString s = Value( Expression::evaluationContext().node ).toString();
		return Value( s );
	}
	return Value( arg( 0 )->evaluate().toString() );
}

Value FunConcat::doEvaluate() const
{
	QString str;
	for ( unsigned int i = 0; i < argCount(); ++i ) {
		str.append( arg( i )->evaluate().toString() );
	}
	return Value( DomString( str ) );
}

Value FunStartsWith::doEvaluate() const
{
	DomString s1 = arg( 0 )->evaluate().toString();
	DomString s2 = arg( 1 )->evaluate().toString();

	if ( s2.isEmpty() ) {
		return Value( true );
	}

	return Value( s1.startsWith( s2 ) );
}

Value FunContains::doEvaluate() const
{
	DomString s1 = arg( 0 )->evaluate().toString();
	DomString s2 = arg( 1 )->evaluate().toString();

	if ( s2.isEmpty() ) {
		return Value( true );
	}

	return Value( s1.contains( s2 ) );
}

Value FunSubstringBefore::doEvaluate() const
{
	QString s1 = arg( 0 )->evaluate().toString();
	QString s2 = arg( 1 )->evaluate().toString();

	if ( s2.isEmpty() ) {
		return Value( "" );
	}

	int i = s1.indexOf( s2 );
	if ( i == -1 ) {
		return Value( "" );
	}

	return Value( DomString( s1.left( i ) ) );
}

Value FunSubstringAfter::doEvaluate() const
{
	QString s1 = arg( 0 )->evaluate().toString();
	QString s2 = arg( 1 )->evaluate().toString();

	if ( s2.isEmpty() ) {
		return Value( s2 );
	}

	int i = s1.indexOf( s2 );
	if ( i == -1 ) {
		return Value( "" );
	}

	return Value( DomString( s1.mid( i + 1 ) ) );
}

Value FunSubstring::doEvaluate() const
{
	QString s = arg( 0 )->evaluate().toString();
	long pos = long( qRound( arg( 1 )->evaluate().toNumber() ) );
	bool haveLength = argCount() == 3;
	long len = -1;
	if ( haveLength ) {
		len = long( qRound( arg( 2 )->evaluate().toNumber() ) );
	}

	if ( pos > long( s.length() ) ) {
		return Value( "" );
	}

	if ( haveLength && pos < 1 ) {
		len -= 1 - pos;
		pos = 1;
		if ( len < 1 ) {
			return Value( "" );
		}
	}

	return Value( DomString( s.mid( pos - 1, len ) ) );
}

Value FunStringLength::doEvaluate() const
{
	if ( argCount() == 0 ) {
		DomString s = Value( Expression::evaluationContext().node ).toString();
		return Value( double( s.length() ) );
	}

	return Value( double( arg( 0 )->evaluate().toString().length() ) );
}

Value FunNormalizeSpace::doEvaluate() const
{
	if ( argCount() == 0 ) {
		DomString s = Value( Expression::evaluationContext().node ).toString();
		return Value( DomString( s.simplified() ) );
	}

	QString s = arg( 0 )->evaluate().toString();
	s = s.simplified();
	return Value( DomString( s ) );
}

Value FunTranslate::doEvaluate() const
{
	QString s1 = arg( 0 )->evaluate().toString();
	QString s2 = arg( 1 )->evaluate().toString();
	QString s3 = arg( 2 )->evaluate().toString();
	QString newString;

	for ( int i1 = 0; i1 < s1.length(); ++i1 ) {
		QChar ch = s1[ i1 ];
		int i2 = s2.indexOf( ch );
		if ( i2 == -1 ) {
			newString += ch;
		} else if ( i2 < s3.length() ) {
			newString += s3[ i2 ];
		}
	}

	return Value( DomString( newString ) );
}

Value FunBoolean::doEvaluate() const
{
	return Value( arg( 0 )->evaluate().toBoolean() );
}

Value FunNot::doEvaluate() const
{
	return Value( !arg( 0 )->evaluate().toBoolean() );
}

Value FunTrue::doEvaluate() const
{
	return Value( true );
}

bool FunTrue::isConstant() const
{
	return true;
}

Value FunLang::doEvaluate() const
{
	QString lang = arg( 0 )->evaluate().toString();

	NodeImpl* node = evaluationContext().node;

    LocalName pln = LocalName::fromString("lang");
    PrefixName xmsnsURI = PrefixName::fromString("xms");
    DOMStringImpl* langNodeValue;

    //NOTE: check this, is it nly ElementImpl or must use NodeImpl::findNextElementAncestor
	while ( node ) {
		NamedAttrMapImpl *attrs = static_cast<ElementImpl*>(node)->attributes( true /* r/o */ );
		
        langNodeValue = attrs->getValue(pln.id(),xmsnsURI);
		if ( langNodeValue ) {
			break;
		}
		node = node->parentNode();
	}

	if ( !langNodeValue ) {
		return Value( false );
	}

	// extract 'en' out of 'en-us'
	QString langNodeValueString = langNodeValue->string();
    langNodeValueString = langNodeValueString.left( langNodeValueString.indexOf( '-' ) );

	return Value( langNodeValueString.toLower() == lang.toLower() );
}

bool FunLang::isConstant() const
{
	return false;
}

Value FunFalse::doEvaluate() const
{
	return Value( false );
}

bool FunFalse::isConstant() const
{
	return true;
}

Value FunNumber::doEvaluate() const
{
	return Value( arg( 0 )->evaluate().toNumber() );
}

Value FunSum::doEvaluate() const
{
	Value a = arg( 0 )->evaluate();
	if ( !a.isNodeset() ) {
		qWarning( "sum() expects <nodeset>" );
		return Value( 0.0 );
	}

	double sum = 0.0;
	const DomNodeList nodes = a.toNodeset();
	foreach( NodeImpl *node, nodes ) {
		sum += Value( stringValue( node ) ).toNumber();
	}
	return Value( sum );
}

Value FunFloor::doEvaluate() const
{
	const double num = arg( 0 )->evaluate().toNumber();

	if ( isnan( num ) || isinf( num ) ) {
		return Value( num );
	}

	return Value( floor( num ) );
}

Value FunCeiling::doEvaluate() const
{
	const double num = arg( 0 )->evaluate().toNumber();

	if ( isnan( num ) || isinf( num ) ) {
		return Value( num );
	}

	return Value( ceil( num ) );
}

Value FunRound::doEvaluate() const
{
	return Value( double( qRound( arg( 0 )->evaluate().toNumber() ) ) );
}

struct FunctionLibrary::FunctionRec
{
	typedef Function *(*FactoryFn )();

	FactoryFn factoryFn;
	Interval args;
};

struct FunctionMapping
{
	const char *name;
	FunctionLibrary::FunctionRec function;
};

static FunctionMapping functions[] = {
	{ "last", { &createFunLast, 0 } },
	{ "last", { &createFunLast, 0 } },
	{ "position", { &createFunPosition, 0 } },
	{ "count", { &createFunCount, 1 } },
	{ "sum", { &createFunSum, 1 } },
	{ "local-name", { &createFunLocalName, Interval( 0, 1 ) } },
	{ "namespace-uri", { &createFunNamespaceURI, Interval( 0, 1 ) } },
	{ "name", { &createFunName, Interval( 0, 1 ) } },

	{ "string", { &createFunString, Interval( 0, 1 ) } },
	{ "concat", { &createFunConcat, Interval( 2, Interval::Inf ) } },
	{ "starts-with", { &createFunStartsWith, 2 } },
	{ "contains", { &createFunContains, 2 } },
	{ "substring-before", { &createFunSubstringBefore, 2 } },
	{ "substring-after", { &createFunSubstringAfter, 2 } },
	{ "substring", { &createFunSubstring, Interval( 2, 3 ) } },
	{ "string-length", { &createFunStringLength, 1 } },
	{ "normalize-space", { &createFunNormalizeSpace, 1 } },
	{ "translate", { &createFunTranslate, 3 } },

	{ "boolean", { &createFunBoolean, 1 } },
	{ "not", { &createFunNot, 1 } },
	{ "true", { &createFunTrue, 0 } },
	{ "false", { &createFunFalse, 0 } },
	{ "lang", { &createFunLang, 1 } },

	{ "number", { &createFunNumber, 1 } },
	{ "floor", { &createFunFloor, 1 } },
	{ "ceiling", { &createFunCeiling, 1 } },
	{ "round", { &createFunRound, 1 } }
};
static const unsigned int numFunctions = sizeof( functions ) / sizeof( functions[ 0 ] );

FunctionLibrary &FunctionLibrary::self()
{
	static FunctionLibrary instance;
	return instance;
}

FunctionLibrary::FunctionLibrary()
{
	for ( unsigned int i = 0; i < numFunctions; ++i ) {
		m_functionDict.insert( functions[ i ].name, functions[ i ].function );
	}
}

Function *FunctionLibrary::getFunction( const char *name,
                                        const QList<Expression *> &args ) const
{
	if ( !m_functionDict.contains( name ) ) {
		qWarning() << "Function '" << name << "' not supported by this implementation.";

		// Return a dummy function instead of 0.
		Function *funcTrue = m_functionDict[ "true" ].factoryFn();
		funcTrue->setName( "true" );
		return funcTrue;
	}

	FunctionRec functionRec = m_functionDict[ name ];
	if ( !functionRec.args.contains( args.count() ) ) {
		qWarning() << "Function '" << name << "' requires " << functionRec.args.asString() << " arguments, but " << args.count() << " given.";
		return 0;
	}

	Function *function = functionRec.factoryFn();
	function->setArguments( args );
	function->setName( name );
	return function;
}
