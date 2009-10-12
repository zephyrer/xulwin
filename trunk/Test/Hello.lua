-- Element:init
-- Element:type
-- Element:parent
-- Element:getAttribute
-- Element:getDocumentAttribute
-- Element:getStyle
-- Element:setAttribute
-- Element:setInnerText
-- Element:innerText
-- Element:getElementById
-- Element:children
-- Element:addEventListener
-- Element:removeEventListener
-- Element:impl
-- Element:addChild
-- Element:removeChild

XULWin.initialize()

el = XULWin.loadApplication("hello")
button = el:getElementById("helloButton")
button:setAttribute("label", "Hacked by Lua!")
XULWin.showModal(el)

XULWin.finalize()
