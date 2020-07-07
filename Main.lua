function Hello(x, y, z)
	local xx = x + 1
	local yy = y + 0.1
	local zz = tostring(x)..tostring(y)
	print("lua hello first  : "..tostring(x)..", "..tostring(y)..", "..tostring(z))
	xx, yy, zz = C_Hello(xx, yy, zz)
	print("lua hello second : "..tostring(xx)..", "..tostring(yy)..", "..tostring(zz)) 
	return 1, 2, 3, 4
end