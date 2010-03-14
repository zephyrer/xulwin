require "rexml/document"

# Usage should be:
# ruby create-element.rb classname
input = ARGV[0]
args = input.split("::")

projectname = "XULWin"
projectpath = "../../#{projectname}/"
classname = args[0]

hppdir = projectpath + "include/#{projectname}/"
cppdir = projectpath + 'src/'

projectname = "#{projectname}_vs80.vcproj"

hppfile = classname + ".h"
hpppath = hppdir + hppfile

cppfile = classname + ".cpp"
cpppath = cppdir + cppfile


File.open("ClassTemplate.h", 'r') do |elHeaderTemplate|
    File.open(hpppath, 'w') do |out|
        while (line = elHeaderTemplate.gets())
            out << line.gsub("{{CLASS_NAME}}", classname).gsub(
                             "{{CLASS_NAME_UPPER}}", classname.upcase())
        end
    end
end

File.open("ClassTemplate.cpp", 'r') do |elCppTemplate|
    File.open(cpppath, 'w') do |out|
        while (line = elCppTemplate.gets())
            out << line.gsub("{{CLASS_NAME}}", classname)
        end
    end
end


system("svn add #{hpppath}")
system("svn add #{cpppath}")

contents = ""

File.open(projectpath + projectname, 'r') do |file|
    doc = REXML::Document.new file

    elementsFolder = doc.elements["VisualStudioProject/Files"]

    newFolder = REXML::Element.new("Filter")
    newFolder.attributes["Name"] = classname
    elementsFolder.insert_after(elementsFolder, newFolder)

    headerElement = REXML::Element.new("File")
    headerElement.attributes["RelativePath"] = ".\\include\\XULWin\\" + hppfile;
    newFolder.insert_after(newFolder, headerElement)

    headerElement = REXML::Element.new("File")
    headerElement.attributes["RelativePath"] = ".\\src\\" + cppfile;
    newFolder.insert_after(newFolder, headerElement)

    contents << doc.to_s
    
    # Prevent Visual Studio Conversion Wizard trigger
    contents.sub!(/'8,00'/, "\"8,00\"")
end

File.open(projectpath + projectname, 'w') do |file|
    file << contents
end

