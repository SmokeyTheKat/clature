"ddScript syntax file


if !exists("main_syntax")
  if exists("b:current_syntax")
    finish
  endif
  let main_syntax = 'clature'
endif

runtime! ~/.vim/syntax/ds.vim

syntax keyword dsLangKeywords		fun sub iso if while for return struct global continue
syntax match dsLangDecNumber 		"\d\+"
syntax match dsLangHexNumber 		"\<0[xX]\x\+\>"
syntax match dsLangBinNumber 		"\<0[bB][01]\+\>"
syntax match dsLangComment		";.*$"
syntax match dsLangName			"[a-zA-Z]"
syntax match dsLangCurlyBrackets	"{"
syntax match dsLangCurlyBrackets	"}"
syntax match dsLangBools		"true"
syntax match dsLangBools		"false"
syntax match dsLangMacros		"/define.*$"
syntax match dsLangOperators		"+"
syntax match dsLangOperators		"+="
syntax match dsLangOperators		"-"
syntax match dsLangOperators		"-="
syntax match dsLangOperators		"*"
syntax match dsLangOperators		"*="
syntax match dsLangOperators		"/"
syntax match dsLangOperators		"/="
syntax match dsLangOperators		"++"
syntax match dsLangOperators		"--"
syntax match dsLangOperators		"="
syntax match dsLangOperators		"=="
syntax match dsLangOperators		"<"
syntax match dsLangOperators		"<<"
syntax match dsLangOperators		"<="
syntax match dsLangOperators		">"
syntax match dsLangOperators		">>"
syntax match dsLangOperators		">="
syntax match dsLangOperators		"|"
syntax match dsLangOperators		"|="
syntax match dsLangOperators		"||"
syntax match dsLangOperators		"&"
syntax match dsLangOperators		"&&"
syntax match dsLangOperators		"&="
syntax match dsLangOperators		"\~"
syntax match dsLangOperators		"!"
syntax match dsLangOperators		"!="
syntax match dsLangOperators		"@"
syntax match dsLangInlineAsm		"\..*	"
hi def link dsLangName			Function
hi def link dsLangComment		Comment
hi def link dsLangKeywords		Keyword
hi def link dsLangDecNumber		Number
hi def link dsLangHexNumber		Number
hi def link dsLangBinNumber		Constant
hi def link dsLangCurlyBrackets		Special
hi def link dsLangBools			Boolean
hi def link dsLangMacros		Define
hi def link dsLangOperators		Operator
hi def link dsLangInlineAsm		Special

let b:current_syntax = "clature"
if main_syntax == 'clature'
	unlet main_syntax
endif
