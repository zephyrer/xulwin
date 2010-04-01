require "rexml/document"
#
# Usage
# ruby CreateElement.rb <component name>
#
# For example:
# ruby CreateElement.rb Label
#
# Generates LabelElement.h, LabelElement.cpp, Label.h and Label.cpp
# and adds these files to the Visual Studio project.
#
if ARGV.length != 1
    puts "Usage: ruby CreateElement.rb <component name>"
    exit
end

def compile_template(template_file, output_file, mapping)
    File.open(template_file, 'r') do |template|
        File.open(output_file, 'w') do |out|
            while (line = template.gets())
                new_line = line
                mapping.each do |key, value|
                    new_line = new_line.gsub(key, value)
                end
                out << new_line
            end
        end
    end
	return output_file
end


def compile_element_header_file(template_header_file, element_name, component_name, hpp_path)
    compile_template(template_header_file,
                     File.join(hpp_path, element_name + ".h"),
                     { "{{ELEMENT_NAME}}" => element_name,
                       "{{ELEMENT_NAME_UPPER}}" => element_name.upcase(),
                       "{{ELEMENT_TYPE}}" => component_name.downcase() })
end


def compile_element_source_file(template_source_file, element_name, component_name, cpp_path)
    compile_template(template_source_file,
                     File.join(cpp_path, element_name + ".cpp"),
                     { "{{ELEMENT_NAME}}" => element_name,
                       "{{COMPONENT_NAME}}" => component_name })
end


def compile_component_header_file(template_header_file, component_name, hpp_path)
    compile_template(template_header_file,
                     File.join(hpp_path, component_name + ".h"),
                     { "{{COMPONENT_NAME}}" => component_name,
                       "{{COMPONENT_NAME_UPPER}}" => component_name.upcase() } )
end


def compile_component_source_file(template_source_file, component_name, cpp_path)
    compile_template(template_source_file,
                     File.join(cpp_path, component_name + ".cpp"),
                     { "{{COMPONENT_NAME}}" => component_name } )
end


def add_file_to_vs_project(filter_node, relative_path)
	element = REXML::Element.new("File")
	element.attributes["RelativePath"] = relative_path
	filter_node.add_element(element)
	puts "Filter node: #{filter_node.to_s}. Path: #{relative_path.to_s}."
end


def add_files_to_vs_project(project_path,
							element_hpp_file,
							element_cpp_file,
							component_hpp_file,
							component_cpp_file)
	contents = ""

	File.open(project_path, 'r') do |file|
		doc = REXML::Document.new file
		doc.get_elements("VisualStudioProject/Files").each do |root_filter|
			root_filter.each_element("Filter[@Name='Elements']") do |elements_filter|
				elements_filter.each_element("Filter[@Name='Header Files']") do |element_headers_filter|
					add_file_to_vs_project(element_headers_filter, ".\\include\\XULWin\\" + element_hpp_file)
				end
				elements_filter.each_element("Filter[@Name='Source Files']") do |element_sources_filter|
					add_file_to_vs_project(element_sources_filter, ".\\src\\" + element_cpp_file)
				end
			end
			
			root_filter.each_element("Filter[@Name='Components']") do |components_filter|
				components_filter.each_element("Filter[@Name='Header Files']") do |component_headers_filter|
					add_file_to_vs_project(component_headers_filter, ".\\include\\XULWin\\" + component_hpp_file)
				end
				components_filter.each_element("Filter[@Name='Source Files']") do |component_sources_filter|
					add_file_to_vs_project(component_sources_filter, ".\\src\\" + component_cpp_file)
				end
			end
		end

		contents << doc.to_s
		
		# Prevent Visual Studio Conversion Wizard trigger
		contents.sub!(/'8,00'/, "\"8,00\"")
		contents.sub!(/'9,00'/, "\"9,00\"")
	end
	
	File.open(project_path, 'w') do |file|
		file << contents
	end
end


def main
	project_path = "../../XULWin/"
	hpp_path = File.join(project_path, "include/XULWin/")
	cpp_path = File.join(project_path, "src/")
	component_name = ARGV[0]
	element_name = ARGV[0] + "Element"
	
	element_hpp_file = compile_element_header_file("ElementTemplate.h", element_name, component_name, hpp_path)
	element_cpp_file = compile_element_source_file("ElementTemplate.cpp", element_name, component_name, cpp_path)
    component_hpp_file = compile_component_header_file("ComponentTemplate.h", component_name, hpp_path)
    component_cpp_file = compile_component_source_file("ComponentTemplate.cpp", component_name, cpp_path)

	system("svn add #{element_hpp_file}")
	system("svn add #{element_cpp_file}")
	system("svn add #{component_hpp_file}")
	system("svn add #{component_cpp_file}")
		
	add_files_to_vs_project(File.join(project_path, "XULWin_vs90.vcproj"),
							File.basename(element_hpp_file),
							File.basename(element_cpp_file),
							File.basename(component_hpp_file),
							File.basename(component_cpp_file))
end

main()




