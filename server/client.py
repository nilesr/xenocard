#!/usr/bin/env python3
import asyncio, aioconsole, json

def handle_input(ii):
	i = ii.split(" ")
	if i[0] == "phase":
		return json.dumps({"method": "set_phase", "phase": i[1]})
	if i[0] == "shuffle":
		return json.dumps({"method": "shuffle"})
	if i[0] == "play":
		return json.dumps({"method": "play", "hand_index": int(i[1]), "position": {"player_side": i[2], "field_segment": i[3], "index": int(i[4])}})
	if i[0] == "discard":
		return json.dumps({"method": "discard", "hand_index": int(i[1])})
	print("Didn't understand command")
	return ii

async def client():
	reader, writer = await asyncio.open_connection("127.0.0.1", 9123)
	def make_inp():
		return asyncio.create_task(aioconsole.ainput(">>> "))
	def make_rd():
		return asyncio.create_task(reader.readline())
	inp = make_inp()
	rd = make_rd()
	while True:
		await asyncio.wait([inp, rd], return_when='FIRST_COMPLETED')
		if inp.done():
			writer.write((handle_input(inp.result()) + "\n").encode())
			await writer.drain()
			inp = make_inp()
		if rd.done():
			print(json.dumps(json.loads(rd.result().decode()), indent=4))
			rd = make_rd()

asyncio.run(client())
