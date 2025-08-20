## Standard Commands
`>` : Increment the data pointer (to point to the next cell to the right).
`<` : Decrement the data pointer (to point to the next cell to the left).
`+` : Increment the byte at the data pointer.
`-` : Decrement the byte at the data pointer.
`.` : Output the byte at the data pointer.
`,` : Accept one byte of input, storing its value in the byte at the data pointer.
`[` : If the byte at the data pointer is zero, then instead of moving the
instruction pointer forward to the next command, jump it forward to the
command after the matching ] command.
`]` : If the byte at the data pointer is nonzero, then instead of moving the
instruction pointer forward to the next command, jump it back to the
command after the matching [ command.

## Custom Layer Commands
`^` : Move to the layer above. If it doesn't exist, a new layer is created.
The size of the new layer is determined by the value of the current
cell in the layer below it.
`v` : Move to the layer below the current one.
`#` : Free the current layer's memory tape. The interpreter then moves to the
layer below at the same pointer position. Layer 0 cannot be freed.

## Usage
```sh
./bf_interpreter <path_to_brainfuck_file.bf>
```

You can use `/challenge/hello_world.bf` to test this wonderful program!
