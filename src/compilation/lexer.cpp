/*
 * File Name: lexer.h
 * Author(s): P. Kamara
 *
 * Lexer.
 */

#include <cwctype>
#include <sstream>
#include "exceptions.h"
#include "compilation/lexer.h"

namespace cherie::compiler
{
	bool lexer::is_end_of_source(size_t ptr) const
	{
		return source_.length() <= 0 || ptr >= source_.length();
	}
	
	types::che_char lexer::peek() const
	{
		return stream_->peek();
	}

	types::che_char lexer::get(const bool skip_whitespace, const bool clear_whitespace)
	{
		if (skip_whitespace)
		{
			while (std::iswspace(peek()))
			{
				get(false, true);
			}
		}

		const types::che_char next_character = stream_->get();
		if (next_character == '\n')
		{
			line_++;
			column_ = 0;
		}
		else if (next_character != eof)
		{
			column_++;
		}

		if (!clear_whitespace)
		{
			current_lexeme_ += next_character;
		}
		return next_character;
	}

	types::string lexer::read_string()
	{
		auto str = std::move(current_lexeme_);
		discard();
		return str;
	}

	long long lexer::read_number(const bool is_hex)
	{
		return is_hex ? std::stoll(read_string(), nullptr, 16) : std::stoll(read_string());
	}

	long double lexer::read_float()
	{
		return std::stold(read_string());
	}

	void lexer::discard()
	{
		current_lexeme_.clear();
	}

	void lexer::get_and_discard()
	{
		get();
		discard();
	}


	void lexer::skip_comment(const bool is_long_comment)
	{
		while (const auto atom = get())
		{
			switch (atom)
			{
				case eof:
				{
					if (is_long_comment)
					{
						return lexer_error("unfinished long comment near line %d", line_);
					}
					return get_and_discard();
				}
				case '*':
				{
					if (is_long_comment && peek() == '/')
					{
						return get_and_discard();
					}
					break;
				}
				case '\n':
				{
					if (!is_long_comment)
					{
						return get_and_discard();
					}
				}
				default:
				{
					break;
				}
			}
		}
	}

	void lexer::tokenize_string_literal(token& token_reference)
	{
		if (peek() == '"')
		{
			token_reference = types::string();
			return get_and_discard();
		}
		
		while (const auto atom = get())
		{
			switch (atom)
			{
				case eof:
				case '\n':
				{
					return lexer_error("unterminated string literal on line %d", line_);
				}
				case '"':
				{
					auto string_literal = read_string().erase(0, 1);
					string_literal.pop_back();
					token_reference = string_literal;
					return;
				}
				default:
				{
					if (std::iswspace(atom) && atom == '\n' || !std::iswprint(atom))
					{
						return lexer_error("unsupported character '%s' on line %d", atom, line_);
					}
					break;
				}
			}
		}
	}

	void lexer::tokenize_number_literal(token& token_reference, const types::che_char first_digit)
	{
		auto is_floating_point_number = false;
		const auto is_hex = first_digit == '0' && peek() == 'x';
		if (is_hex)
		{
			get();
		}

		while (const auto atom = peek())
		{
			switch (atom)
			{
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
				{
					get();
					break;
				}
				case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
				{
					if (!is_hex || is_floating_point_number)
					{
						return lexer_error("unexpected character in number literal on line %s", line_);
					}
					get();
					break;
				}
				case '.':
				{
					if (is_hex || is_floating_point_number)
					{
						return lexer_error("invalid float literal on line %d", line_);
					}
					is_floating_point_number = true;
					get();
					break;
				}
				default:
				{
					if (atom == eof || std::iswspace(atom) || std::ispunct(atom))
					{
						if (is_floating_point_number)
						{
							token_reference = read_float();
						}
						else
						{
							token_reference = read_number(is_hex);
						}
						return;
					}
					return lexer_error("unexpected character in number literal on line %s", line_);
				}
			}
		}
	}

	token_type lexer::tokenize_symbol(const types::che_char symbol)
	{
		auto return_type = token_type::NONE;
		if (const auto next_atom = peek(); next_atom != eof && std::iswpunct(next_atom))
		{
			switch (next_atom)
			{
				case '=':
				{
					switch (symbol)
					{
						case '=': return_type = token_type::EQUALS; break;
						case '+': return_type = token_type::ADD_ASSIGN; break;
						case '-': return_type = token_type::SUBTRACT_ASSIGN; break;
						case '/': return_type = token_type::DIVIDE_ASSIGN; break;
						default: break;
					}
					break;
				}
				case '+':
				{
					if (symbol == '+') return_type = token_type::INCREMENT;
					break;
				}
				case '-':
				{
					if (symbol == '-') return_type = token_type::DECREMENT;
					break;
				}
				case '>':
				{
					if (symbol == '-') return_type = token_type::ARROW;
					break;
				}
				default: break;
			}
			
			if (return_type != token_type::NONE)
			{
				get_and_discard();
				return return_type;
			}
		}

		discard();
		switch (symbol) // check single symbols
		{
			case '=': return token_type::EQUALS;
			case '+': return token_type::ADD;
			case '-': return token_type::SUBTRACT;
			case '*': return token_type::MULTIPLY;
			case '/': return token_type::DIVIDE;
			case '(': return token_type::OPEN_PARENTHESIS;
			case ')': return token_type::CLOSE_PARENTHESIS;
			case '{': return token_type::OPEN_BRACE;
			case '}': return token_type::CLOSE_BRACE;
			case '[': return token_type::OPEN_BRACKET;
			case ']': return token_type::CLOSE_BRACE;
			case ',': return token_type::COMMA;
			case ':': return token_type::COLON;
			case ';': return token_type::SEMICOLON;
			case '?': return token_type::QUESTION_MARK;
			case '!': return token_type::EXCLAMATION_MARK;
			case '.': return token_type::DOT;
			default: lexer_error("unknown symbol '%s' on line %d, column %d", symbol, line_,
				column_);
		}
		return token_type::NONE;
	}

	bool is_not_identifier(types::che_char atom)
	{
		return atom != lexer::eof && (std::iswspace(atom) || !std::iswalnum(atom)) && atom != '_';
	}
	
	token_type lexer::tokenize_identifier_or_keyword(token& token_reference, types::che_char first_character)
	{
		auto can_be_keyword = first_character != '_';

		if (is_not_identifier(peek()))
		{
			const auto identifier = read_string();
			token_reference = identifier;
			return token_type::IDENTIFIER;
		}

		types::string test_string;
		test_string += first_character;
		
		while (const auto atom = get())
		{			
			if (can_be_keyword)
			{
				if (atom == '_')
				{
					can_be_keyword = false;
					continue;
				}
				
				test_string += atom;
				if (const auto reserved_keyword_pair = keyword_map.find(test_string);
					reserved_keyword_pair != keyword_map.end())
				{
					if (std::iswspace(peek()) || std::iswpunct(peek()))
					{
						discard();
						return reserved_keyword_pair->second;
					}
					can_be_keyword = false;
				}
			}

			if (is_not_identifier(peek()))
			{
				const auto identifier = read_string();
				token_reference = identifier;
				return token_type::IDENTIFIER;
			}
		}
		return token_type::NONE;
	}

	token_type lexer::tokenize(token& token_reference)
	{
		if (const auto atom = get(true))
		{
			switch (atom)
			{
				case eof: return token_type::EOF;
				case '\'': // character literal
				{
					token_reference = get();
					if (get() == '\'')
					{
						return token_type::LITERAL;
					}
					lexer_error("expected closing ' on line %d", line_);
					return token_type::NONE;
				}
				case '"': // string literal
				{
					tokenize_string_literal(token_reference);
					return token_type::LITERAL;
				}
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9': // number literal
				{
					tokenize_number_literal(token_reference, atom);
					return token_type::LITERAL;
				}
				case '/':
				{
					if (const auto peeked_character = peek(); peeked_character == '*' || peeked_character == '/')
					{
						skip_comment(peeked_character == '*');
						return next_token();
					}
				}
				default:
				{
					if (atom != '_' && std::iswpunct(atom))
					{
						return tokenize_symbol(atom);
					}
					return tokenize_identifier_or_keyword(token_reference, atom);
				}
			}
		}
		return token_type::NONE; // Should never be reached.
	}
	
	token_type lexer::next_token()
	{
		current_token_ = std::monostate();
		auto peeked_type = peeked_token_type_;
		if (peeked_type != token_type::NONE)
		{
			current_token_ = peeked_token_; // move lookahead into current
			peeked_token_ = std::monostate();
			peeked_token_type_ = token_type::NONE;
			return peeked_type;
		}
		return tokenize(current_token_);
	}

	token_type lexer::peek_token()
	{
		if (peeked_token_type_ == token_type::NONE)
		{
			peeked_token_type_ = tokenize(peeked_token_);
			return peeked_token_type_;
		}
		peeked_token_ = std::monostate();
		lexer_error("attempt to peek token, current peeked token is not cleared");
		return token_type::NONE;
	}

	lexer::lexer(const types::string source)
		: source_(source)
	{
		stream_ = std::make_unique<types::stringstream>(source);
	}
}
