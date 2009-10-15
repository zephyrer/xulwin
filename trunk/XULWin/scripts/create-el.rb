hppdir = ARGV[0]
cppdir = ARGV[1]
classname = ARGV[2]

puts "Classname: " + classname
puts "Element name: " + classname.downcase()
puts "hppdir: " + hppdir
puts "cppdir: " + cppdir


outFile = "test.h"
File.open("ElementTemplate.h", 'r') do |elHeaderTemplate|
    File.open(hppdir + classname + ".h", 'w') do |out|
        while (line = elHeaderTemplate.gets())
            out << line.gsub("{{ELEMENT_NAME}}", classname).gsub(
                             "{{ELEMENT_NAME_UPPER}}", classname.upcase()).gsub(
                             "{{ELEMENT_TYPE}}", classname.downcase())
        end
    end
end

File.open("ElementTemplate.cpp", 'r') do |elCppTemplate|
    File.open(cppdir + classname + ".cpp", 'w') do |out|
        while (line = elCppTemplate.gets())
            out << line.gsub("{{ELEMENT_NAME}}", classname)
        end
    end
end

File.open("ElementImplTemplate.h", 'r') do |template|
    File.open(hppdir + classname + "Impl.h", 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{ELEMENTIMPL_NAME}}", classname + "Impl").gsub(
                             "{{ELEMENTIMPL_NAME_UPPER}}", (classname + "Impl").upcase())
        end
    end
end

File.open("ElementImplTemplate.cpp", 'r') do |template|
    File.open(cppdir + classname + "Impl.cpp", 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{ELEMENTIMPL_NAME}}", classname + "Impl");
        end
    end
end

