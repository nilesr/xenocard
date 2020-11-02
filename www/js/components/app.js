class App extends React.Component {
	constructor(props) {
		super(props);
		const ws = new WebSocket('ws://localhost:8080/');
		ws.onopen = () => this.setState({connected: true});
		ws.onclose = () => alert('Disconnected');
		ws.onmessage = (event) => this.onMessage(JSON.parse(event.data));
		this.state = {
			connected: false,
			ws: ws,
			log: [],
			player: null,
			turn: 0,
			phase: null,
			field: null,
			hand: [],
			junk: [],
			deck_size: 0,
			lost_size: 0,
			enemy_hand_size: 0,
			enemy_deck_size: 0,
			enemy_junk_size: 0,
			enemy_lost_size: 0,
			selection_state: null,
			selection_hand_index: -1,
		};
	}
	render() {
		if (!this.state.connected) {
			return "Waiting for connection to open (if it fails you may be staring at this forever)";
		}
		if (this.state.player == null) {
			return "Connected, waiting for a second player";
		}
		return <div>
			{/* TODO extract into Log if I'm going to keep it */}
			<ul>
				{this.state.log.map((m, i) => 
					<li key={i}><pre>{JSON.stringify(m)}</pre></li>
				)}
			</ul>
			<Field /> {/* TODO */}
			<div style={{
				position: "absolute",
				bottom: 0,
				left: 0,
				width: "100%",
				height: "200px",
				display: 'grid',
				gridTemplateColumns: "150px 300px 1fr",
				gridTemplateRows: '100%',
			}}>
				<div>
					<Phases player={this.state.player} phase={this.state.phase} />
				</div>
				<div>
					<Buttons
						player={this.state.player}
						phase={this.state.phase}
						sendInstruction={this.sendInstruction} />
				</div>
				<div>
					<Hand
						cards={this.state.hand}
						onClick={this.onHandClick}
						selectedIndex={this.state.selection_hand_index}/>
				</div>
			</div>
		</div>;
	}
	onMessage(data) {
		// debug
		this.setState(s => {return {log: s.log.concat([data])}; });
		if (data.event) {
			if (data.event == "error") {
				this.recvError(data);
			} else {
				this.recvNotice(data);
			}
		} else {
			this.recvGame(data);
		}
	}
	recvError(data) {
		alert(data.error);
	}
	recvNotice(data) {
		console.log(data);
	}

	sendInstruction = (method, extras) => {
		extras ||= {};
		extras.method = method;
		const raw = JSON.stringify(extras);
		if (raw.length > 4096) {
			alert(method + " message is too long to be sent to the server - logged to the console.");
			console.error(extras);
		}
		this.state.ws.send(raw);
	}
	recvGame(game) {
		this.setState({
			player: game.player,
			turn: game.turn,
			phase: game.phase,
			field: game.field,
			hand: game.hand,
			junk: game.junk,
			deck_size: game.deck_size,
			lost_size: game.lost_size,
			enemy_hand_size: game.enemy_hand_size,
			enemy_deck_size: game.enemy_deck_size,
			enemy_junk_size: game.enemy_junk_size,
			enemy_lost_size: game.enemy_lost_size,
		})
	}
	onHandClick = (handIndex) => {
		this.setState(s => {
			if (s.selection_state == "move") {
				// no effect
				return {};
			}
			if (s.selection_state == "play") {
				// no effect, cancel selection
				return {selection_state: null, selection_hand_index: -1};
			}
			const card = s.hand[handIndex];
			if (s.phase == s.player + "_ADJUST") {
				if (!confirm("Are you sure you want to discard this card: " + card.name)) return;
				this.sendInstruction("discard", {"hand_index": handIndex});
				return {};
			}
			if (card.type == "EVENT") {
				// TODO handle use card effect
				alert("TODO");
				throw "TODO"; // to get the stack trace into the console
			}
			return {
				selection_state: "play",
				selection_hand_index: handIndex,
			};
		})
	}
};

window.App = App;

