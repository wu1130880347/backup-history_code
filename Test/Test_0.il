procedure(trTrace(fun @rest args)
        printf("Calling %s passing %L" fun args)
        result=apply(fun args)
        printf("\nReturning from %s with %L\n" fun args)
        result
);end procedure