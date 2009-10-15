require "rexml/document"
projectpath = '../../XULWin/'
hppdir = projectpath + 'include/XULWin/'
cppdir = projectpath + 'src/'

classname = ARGV[0]
projectname = "XULWin_vs80.vcproj"

hppfile = classname + ".h"
hpppath = hppdir + hppfile

cppfile = classname + ".cpp"
cpppath = cppdir + cppfile

hppimplfile = classname + "Impl.h"
hppimplpath = hppdir + hppimplfile

cppimplfile = classname + "Impl.cpp"
cppimplpath = cppdir + cppimplfile


outFile = "test.h"
File.open("ElementTemplate.h", 'r') do |elHeaderTemplate|
    File.open(hpppath, 'w') do |out|
        while (line = elHeaderTemplate.gets())
            out << line.gsub("{{ELEMENT_NAME}}", classname).gsub(
                             "{{ELEMENT_NAME_UPPER}}", classname.upcase()).gsub(
                             "{{ELEMENT_TYPE}}", classname.downcase())
        end
    end
end

File.open("ElementTemplate.cpp", 'r') do |elCppTemplate|
    File.open(cpppath, 'w') do |out|
        while (line = elCppTemplate.gets())
            out << line.gsub("{{ELEMENT_NAME}}", classname)
        end
    end
end

File.open("ElementImplTemplate.h", 'r') do |template|
    File.open(hppimplpath, 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{ELEMENTIMPL_NAME}}", classname + "Impl").gsub(
                             "{{ELEMENTIMPL_NAME_UPPER}}", (classname + "Impl").upcase())
        end
    end
end

File.open("ElementImplTemplate.cpp", 'r') do |template|
    File.open(cppimplpath, 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{ELEMENTIMPL_NAME}}", classname + "Impl");
        end
    end
end

system("svn add #{hpppath}")
system("svn add #{hppimplpath}")
system("svn add #{cpppath}")
system("svn add #{cppimplpath}")

contents = ""

File.open(projectpath + projectname, 'r') do |file|
    doc = REXML::Document.new file

    elementsFolder = doc.elements["VisualStudioProject" +
                                  "/Files"]

    newFolder = REXML::Element.new("Filter")
    newFolder.attributes["Name"] = classname
    elementsFolder.insert_after(elementsFolder, newFolder)

    headerElement = REXML::Element.new("File")
    headerElement.attributes["RelativePath"] = ".\\include\\XULWin\\" + hppfile;
    newFolder.insert_after(newFolder, headerElement)

    headerElement = REXML::Element.new("File")
    headerElement.attributes["RelativePath"] = ".\\src\\" + cppfile;
    newFolder.insert_after(newFolder, headerElement)

    headerElement = REXML::Element.new("File")
    headerElement.attributes["RelativePath"] = ".\\include\\XULWin\\" + hppimplfile;
    newFolder.insert_after(newFolder, headerElement)

    headerElement = REXML::Element.new("File")
    headerElement.attributes["RelativePath"] = ".\\src\\" + cppimplfile;
    newFolder.insert_after(newFolder, headerElement)

    contents << doc.to_s
    
    # Prevent Visual Studio Conversion Wizard trigger
    contents.sub!(/'8,00'/, "\"8,00\"")
end

File.open(projectpath + projectname, 'w') do |file|
    file << contents
end

