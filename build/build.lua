assert(_VERSION == "Lua 5.4", "project is optimised for lua 5.4.8 and is not guaranteed to work with other versions")

if (arg[1] == nil) then dofile("build/build_docs.lua") ; io.write("awaiting argument, type exit to exit :\n") ; arg[1] = io.read() end
end_build  = function() io.write(arg[1], " successful\n") ; os.exit() end
if  arg[1]  == "exit" or arg[1] == "90" then end_build() end

local operating_system <const> = 3 * select(3, os.execute("cls"))
parse_args = function()
  if arg[1]  == "exit" or arg[1] == "90" then end_build() end

  local out = { 0, 0, 0 } -- b, r, c

  if arg[1]  == "build" then io.write("building :\n")                       ; out[1] = 1 + operating_system ; return out end
  if arg[1]  == "run"   then io.write("building and running :\n")           ; out[2] = 2 + operating_system ; return out end
  if arg[1]  == "clean" then io.write("building, running and cleaning :\n") ; out[3] = 3 + operating_system ; return out end

  for i = 1, #arg[1] do if string.sub(arg[1], i, i) == 'b' then out[1] = 1 + operating_system ; break end end
  for i = 1, #arg[1] do if string.sub(arg[1], i, i) == 'r' then out[2] = 2 + operating_system ; break end end
  for i = 1, #arg[1] do if string.sub(arg[1], i, i) == 'c' then out[3] = 3 + operating_system ; break end end
  return out
end
local build_args = parse_args()

local main_cpp <const> = "build/main.cpp"
local main_out <const> = "bin/main"

local build_project <const> =
{
  "g++ -Wall -Werror -Wextra -pedantic-errors -o " .. main_out .. ".exe " .. main_cpp,
  "call \"" .. main_out .. ".exe\"",
  "call bin/clean",
  "linux build",
  "linux build and run",
  "linux build run and clean",
  [0] = ""
}

for i = 1, #build_args do
  io.write(build_project[build_args[i]], '\n')
  os.execute(build_project[build_args[i]])
end

end_build()