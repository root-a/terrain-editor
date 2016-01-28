


function onenter(eid,othereid)
	if getcategory(othereid) == "Player" then
		artefact = getentitybyname("Artefact")
		playanimclip(artefact,"artefactanim",0,false)			
	end
end;
