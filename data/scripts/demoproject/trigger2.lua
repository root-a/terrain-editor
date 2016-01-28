


function onenter(eid,othereid)
	if getcategory(othereid) == "Player" then
		door = getentitybyname("door")
		playanimclip(door,"opendoor",0,false)	
		box = getentitybyname("pad2")
		playanimclip(box,"pad2down",0,false)			
	end
end;

function onexit(eid,othereid)
	if getcategory(othereid) == "Player" then
		box = getentitybyname("pad2")
		playanimclip(box,"pad2up",0,false)	
	end
end;