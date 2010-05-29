require "rexml/document"


# Usage should be:
# ruby create-element.rb classname
input = ARGV[0]
args = input.split("::")
$classname = args[0]


def getSolutionDir
  "."
end

def getProjectName
  "XULWin"
end

def getProjectDir
  projDir = File.join(getSolutionDir, getProjectName)
  puts "Project dir is #{projDir}"
  return projDir
end

def getIncludeDir
  File.join(getProjectDir, "include", getProjectName)
end

def getSrcDir
  srcDir = File.join(getProjectDir, "src")
  puts "Src dir is: #{srcDir}"
  return srcDir
end

def getHeaderFileName(classname)
  "#{classname}.h"
end

def getSourceFileName(classname)
  "#{classname}.cpp"
end

def getHeaderFilePath(classname)
  File.join(getIncludeDir, getHeaderFileName(classname))
end

def getSourceFilePath(classname)
  File.join(getSrcDir, getSourceFileName(classname))
end

def getTemplateHeaderPath
  File.join(getSolutionDir, "Scripts", "CreateClass", "ClassTemplate.h")
end

def getTemplateSourcePath
  File.join(getSolutionDir, "Scripts", "CreateClass", "ClassTemplate.cpp")
end


def generateHeaderFile(classname)
  File.open(getTemplateHeaderPath, 'r') do |headerTemplate|
    File.open(getHeaderFilePath(classname), 'w') do |out|
      while (line = headerTemplate.gets())
        out << line.gsub("{{CLASS_NAME}}", classname).
                    gsub("{{CLASS_NAME_UPPER}}", classname.upcase)
      end
    end
  end
end


def generateSourceFile(classname)
  File.open(getTemplateSourcePath, 'r') do |sourceTemplate|
    File.open(getSourceFilePath(classname), 'w') do |out|
      while (line = sourceTemplate.gets())
        out << line.gsub("{{CLASS_NAME}}", classname)
      end
    end
  end
end


def addToSvn(filePath)
  system("svn add #{filePath}")
end


def getProjectVSFileName
  "#{getProjectName}_vs90.vcproj"
end


def updateVisualStudioProject(classname)
  contents = ""
  projectFilePath = File.join(getProjectDir, getProjectVSFileName)
  File.open(projectFilePath, 'r') do |file|
    doc = REXML::Document.new file
    elementsFolder = doc.elements["VisualStudioProject/Files"]

    headerFile = REXML::Element.new("File")
    headerFilePath = getHeaderFilePath(classname).gsub(/\//, "\\")
    puts "Header file path is #{headerFilePath}."
    puts "Header file path to_s is #{headerFilePath.to_s}."
    headerFile.attributes["RelativePath"] = headerFilePath.to_s.sub(/XULWin\\/, "")
    elementsFolder.add_element(headerFile)

    sourceFile = REXML::Element.new("File")
    sourceFilePath = getSourceFilePath(classname).gsub(/\//, "\\")
    puts "Source file path is #{sourceFilePath}."
    puts "Source file path .to_s is #{sourceFilePath.to_s}."
    sourceFile.attributes["RelativePath"] = sourceFilePath.to_s.sub(/XULWin\\/, "")
    elementsFolder.add_element(sourceFile)

    contents << doc.to_s
    
    # Prevent Visual Studio Conversion Wizard trigger
    contents.sub!(/'8,00'/, "\"8,00\"")
    contents.sub!(/'9,00'/, "\"9,00\"")
  end
    
  # Update the VS project file
  File.open(projectFilePath, 'w') do |file|
    file << contents
  end
end


def main(classname)
  generateHeaderFile(classname)
  generateSourceFile(classname)
  addToSvn(getHeaderFilePath(classname))
  addToSvn(getSourceFilePath(classname))
  updateVisualStudioProject(classname)
end

main($classname)
