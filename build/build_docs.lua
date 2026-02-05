io.write("build.lua expects an argument :\n")

local cmds = 
[[
b build - rebuild the project
r run   - build && run the generated executable
]]
--c clean - build && run && delete the executable
io.write(cmds, '\n')

io.write("each step can be combined as initials in any combination : b, br, bc and so on\n")
io.write("no matter the order these steps will be executed in the order build, run, clean\n")

local versions = 
[[
lua 5.4.8
g++ 8.1.0
]]
io.write(versions, '\n')