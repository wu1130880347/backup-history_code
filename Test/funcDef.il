procedure(trTrace(fun @rest args)
	printf("Calling %s passing %L" fun args)
	result=apply(fun args)
	printf("\nReturning from %s with %L\n" fun args)
	result
);end procedure

defun( printName (@optional (name "vivienluo") (age 20))
	printf("hello,%s! Are you %d years old?\n" name age)
)

defun( printName2 (@key (name "vivienluo") (age 20))
	printf("hello,%s! Are you %d years old?\n" name age)
)