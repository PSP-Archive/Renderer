#trig.rb: Generate C files with tables for sin, cos and tan [0-360]

f = File.open("trig.c", "w")

def d2r(d)
	return Math::PI * d / 180.0
end

def round(blah)
	return (blah * 10000000).floor / 10000000.0
end

f.puts("float sintab[] = {")
# write out 10 sines
36.times do |x|
	10.times do |y|
		f.print(round(Math.sin(d2r(10 * x + y))).to_s + "f,")
	end
	f.print("\n")
end

f.puts("};\n\n")

f.puts("float costab[] = {")
# write out 10 sines
36.times do |x|
	10.times do |y|
		f.print(round(Math.cos(d2r(10 * x + y))).to_s + "f,")
	end
	f.print("\n")
end

f.puts("};\n\n")

f.puts("float tantab[] = {")
# write out 10 sines
36.times do |x|
	10.times do |y|
		f.print(round(Math.tan(d2r(10 * x + y))).to_s + "f,")
	end
	f.print("\n")
end

f.puts("};\n\n")

f.puts("float sin(int x) {if (x < 0) x = 360 + x;return sintab[x % 360];}\n")
f.puts("float cos(int x) {if (x < 0) x = 360 + x;return costab[x % 360];}\n")
f.puts("float tan(int x) {if (x < 0) x = 360 + x;return tantab[x % 360];}\n")

f.close

