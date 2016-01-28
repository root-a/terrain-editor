function onenter(eid,othereid)
	if getcategory(othereid) == "Player" then
		for i=1,9 do
			name = "death"..i
			trap = getentitybyname(name)	
			playanimclip(trap,"deathstart",1,false)
			playanimclip(trap,"deathloop",0,true)
		end
	end
end;

function onexit(eid,othereid)
	if getcategory(othereid) == "Player" then
		trap = getentitybyname("death1")	
		playanimclip(trap,"deathend",0,false)
		trap = getentitybyname("death2")	
		playanimclip(trap,"deathend",0,false)
	end
end;
	