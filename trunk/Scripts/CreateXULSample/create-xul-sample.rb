require "rexml/document"

appName = ARGV[0]
srcPath = "CreateXULSample/"
targetPath = "../../xulrunnersamples/#{appName}"
system("svn export #{srcPath} #{targetPath}")

File.open(srcPath + "application.ini", 'r') do |template|
    File.open(targetPath + "application.ini", 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{APP_NAME}}", appName).gsub(
                             "{{APP_NAME_DOWNCASE}}", classname.downcase())
        end
    end
end

