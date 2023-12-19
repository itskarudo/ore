# Ore Language Specification

### Variables
```python
a = 5
global b = 6
```

### Comments
```lua
-- This is comment
```

### Primitive data types
```python
num, str, bool
```

### Data structures
```lua
array = [1, "sloopie", false]
array[0] = 1

map = {a: 5, b: true}

map.a = function (x) {
    print("x is " .. x) -- "x is 5"
  }

map.a(5)
```

### Modules
```js
time = import("time")
pages = import("./pages")

export a = 5
b = 7
c = false

export {
    b,
    c
}
```

### Control flow
```rust
while i > 5 {
  i += 1
}

for i in (1, 5, 2) {
  print(i)
}

do {
  print(i)
  i += 1
} while i > 5

if pp > dd {

} else if pp < dd {

} else {

}

switch a {
  1: {
      
  }
  2,3,4: {

  }
  default: {

  }
}
```

### Functions
```lua
a = function(a, b, x = 5) {
  print(a,b,x)
}

function b(a, ...) { -- varargs
  print(a, ...)
}
```

### Operators
#### Arithmatic

```lua
  -- Addition: +
  -- Subtraction: -
  -- Multiplication: *
  -- Division: /
  -- Integer division: //
  -- Exponentiation: **
  -- Modulus: %
```

#### Assignment
```python
  a += b
  a -= b
  ...
```

#### Logical
```lua
a = true and false
a = true or false
a = true xor false
a = not true
```

#### Bitwise
```lua
a = 1 & 2 -- and
a = 1 | 2 -- or
a = ~1    -- not
a = 1 ^ 2 -- xor
a = 1 << 2 -- shift left
a = 1 >> 2 -- shift right
a = <4    -- MSB
a = >4    -- LSB
```

### String operations
```lua
a = "zoubi " .. "zou" -- concatenation
strlen = #a
t = [1,2,3]
tblen = #t
```
