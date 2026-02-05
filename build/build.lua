if (arg[1] == nil) then dofile("build/build_docs.lua") end ; arg[1] = io.read()
assert(_VERSION == "Lua 5.4", "project is optimised for lua 5.4.8 and is not guaranteed to work with other versions")

if arg[1] == "exit" or arg[1] == "90" then os.exit() end

local operating_system = 0
for i = 1, 1 do local bool, str, val = os.execute("cls") ; operating_system = val end

local build_project <const> =
{
  "g++ -Wall -Werror -Wextra -pedantic-errors -o ../bin/main main.cpp",
  "",
  "call bin\\main",
  "",
  "del bin\\main",
  "",
}

local cmd =
{
  ["build"] = 1,
  ["run"]   = 3,
  ["clean"] = 5,
}

for i = operating_system, #cmd, 2 do os.execute(build_project[cmd[arg[1 + operating_system]]]) end

io.write(arg[1], " successful\n")