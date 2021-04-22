
local CSV = {}

function CSV:loadcsv( src )
	-- Load File
	local file = assert(io.open(src, 'r'))
	local content = file:read("*all")
	file:close()
	-- Read Word One By One
	local c = content -- content
	local p = 0 -- content check pos
	local w = "" -- content check word
	local w_next = "" -- content check word(next one)
	local x = 1 -- content table x
	local y = 1 -- content table y

	local opt = {} -- output table
	opt[y] = {}
	opt[y][x] = ""

	local in_quote = false -- quote mode
	while true do
		p = p + 1
		w = string.sub(c,p ,p )
		w_next = string.sub(c,p+1,p+1)
		if w == ',' then
			if in_quote then
				opt[y][x] = opt[y][x]..w
			else
				x = x + 1
				opt[y][x] = ""
			end
		elseif w == '"' then
			if in_quote then
				if w_next == '"' then
					opt[y][x] = opt[y][x]..w
					p = p + 1
				else
					in_quote = false
				end
			else
				in_quote = true
			end
		elseif w == '\n' then
			if in_quote then
				opt[y][x] = opt[y][x]..w
			else
				y = y + 1
				x = 1
				opt[y] = {}
				opt[y][x] = ""
			end
		elseif w == "" or w == nil then
			opt[y][x] = nil
			break
		else
			opt[y][x] = opt[y][x]..w
		end
	end
	return opt
end

function CSV:new(args)
   local new = { }
   if args then
      for key, val in pairs(args) do
         new[key] = val
      end
   end
   return setmetatable(new, CSV)
end

CSV.__index = CSV
return CSV:new()
