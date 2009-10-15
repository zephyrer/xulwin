require "rexml/document"

appName = ARGV[0]
srcPath = "XULSampleTemplate/"
targetPath = "../../xulrunnersamples/#{appName}/"
system("svn export #{srcPath} #{targetPath}")

applicationIniFile = "application.ini"
File.open(srcPath + applicationIniFile, 'r') do |template|
    File.open(targetPath + applicationIniFile, 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{APP_NAME}}", appName).gsub(
                             "{{APP_NAME_DOWNCASE}}", appName.downcase())
        end
    end
end

manifestFile = "chrome/chrome.manifest"
File.open(srcPath + manifestFile, 'r') do |template|
    File.open(targetPath + manifestFile, 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{APP_NAME}}", appName).gsub(
                             "{{APP_NAME_DOWNCASE}}", appName.downcase())
        end
    end
end

xulFile = "chrome/content/main.xul"
File.open(srcPath + xulFile, 'r') do |template|
    File.open(targetPath + xulFile, 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{APP_NAME}}", appName).gsub(
                             "{{APP_NAME_DOWNCASE}}", appName.downcase())
        end
    end
end

prefsFile = "defaults/preferences/prefs.js"
File.open(srcPath + prefsFile, 'r') do |template|
    File.open(targetPath + prefsFile, 'w') do |out|
        while (line = template.gets())
            out << line.gsub("{{APP_NAME}}", appName).gsub(
                             "{{APP_NAME_DOWNCASE}}", appName.downcase())
        end
    end
end

system("svn add #{targetPath}")
