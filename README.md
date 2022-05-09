# Oper

Oper is an esoteric-ish programming language written in c++. It as named after its core element - operators. Unlike traditional programming languages, oper does not have subroutines, and instead you must write custom operators to break up a program.

## Build
Use make:
```bash
make all
```

## Testing
A custom testing format and tests are included in the repo. To run them, run:
```bash
make test
```


## Syntax

```python
#hello world in oper
print "Hello, world!"

#any block literal will be executed
{
    print "In a block"
}

#traditional assignments, python-style casts
i = (int (input "enter a number: "))

#bool * anything will return none if the bool is false, and the other value if true
#this is used for if statements
(i == 10) * {
    print "i is 10"
}
```

## Contributing
Open an issue if you find a bug (I'm sure there are many to be found). No guarantees that I will check them though.


## License
[MIT](https://choosealicense.com/licenses/mit/)
