#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include "lexer.h"
#include "expression_evaluator.h"
#include "renderer.h"

namespace jinja2
{
class ExpressionParser
{
public:
    ExpressionParser();
    RendererPtr Parse(LexScanner& lexer);
    ExpressionEvaluatorPtr<FullExpressionEvaluator> ParseFullExpression(LexScanner& lexer);
private:
    ExpressionEvaluatorPtr<Expression> ParseLogicalNot(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseLogicalOr(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseLogicalAnd(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseLogicalCompare(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseStringConcat(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseMathPow(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseMathMulDiv(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseMathPlusMinus(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseUnaryPlusMinus(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseValueExpression(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseBracedExpressionOrTuple(LexScanner& lexer);
    ExpressionEvaluatorPtr<Expression> ParseDictionary(LexScanner& lexer);
    ExpressionEvaluatorPtr<SubscriptExpression> ParseSubsicpt(LexScanner& lexer, const std::vector<std::string>& valueRef);
    ExpressionEvaluatorPtr<Expression> ParseCall(LexScanner& lexer, const std::vector<std::string>& valueRef);
    std::unordered_map<std::string, ExpressionEvaluatorPtr<>> ParseCallParams(LexScanner& lexer, bool& isValid);
    std::vector<std::string> ParseValueRef(LexScanner& lexer);
    ExpressionEvaluatorPtr<ExpressionFilter> ParseFilterExpression(LexScanner& lexer);
    ExpressionEvaluatorPtr<IfExpression> ParseIfExpression(LexScanner& lexer);

private:
    ComposedRenderer* m_topLevelRenderer;
};

} // jinja2

#endif // EXPRESSION_PARSER_H
