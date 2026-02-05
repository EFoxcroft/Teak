naiv = function(str)
  --int c = str.charAt(i) ; c = c ^ (i + i) + str.length() ; out = out + (char) c;
  local out = ""
  for i = 1, #str do out = out .. string.char(string.byte(string.sub(str, i, i)) ~ ((i + i) + #str)) end
  return out
end