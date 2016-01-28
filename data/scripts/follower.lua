-- this is the default script with some predefined callbacks
-- uncomment the callbacks you need

-- run when the entity is created
function oninit(eid)	
other = getentitybyname("dummychar")
	movefollow(eid,other,1.0)
end

-- run every frame
--function onframe(eid)
--end

-- run upon a collision event
--function oncollision(eid, othereid, point, normal)
--end

-- input event has happened
-- code is a string describing the key or mouse button, up is true for KeyUp and false for KeyDown
--function oninput(eid, code, up)

--end

-- if your model has animevents (and you activated feedback for them)
--function onanimevent(eid,event)
--end

--end



-- For ScriptingTriggerProperties some additional callbacks are available:

-- runs when entering trigger
--function onenter(eid,othereid)
--end

-- runs every frame inside object
--function oninside(eid, othereid) 
--end

-- runs when exiting trigger
--function onexit(eid, othereid)
--end

-- if the entity has the interactproperty these additional callbacks are available:

-- the mouse went over the gameentity
--function onmouseenter(eid)
--end

-- the mouse left the gameentity
--function onmouseleave(eid)
--end

-- the mouse was clicked/released while over the game entity
function onmousebutton(eid,button,down)
other = getentitybyname("dummychar")
	movefollow(eid,other,1.0)
end