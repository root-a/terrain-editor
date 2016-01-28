

function onenter(eid,othereid)
	if getcategory(othereid) == "Player" then
		othertrigger = getentitybyname("trigger2")	
		settriggeractive(othertrigger,true)
		box = getentitybyname("pad1")
		playanimclip(box,"pad1down",0,false)	
		pos = getattribute(othertrigger,"Transform")
		addattachment(othereid,"LeftHand","examples/box",-1)		
	end
end;

function oninside(eid, othereid)
 if keypressed("Y") then
	playerdeath(othereid)
	end
end;

function onexit(eid,othereid)
	if getcategory(othereid) == "Player" then
		box = getentitybyname("pad1")
		playanimclip(box,"pad1up",0,false)	
		clearattachments(othereid)
	end
end;