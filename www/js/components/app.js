class App extends React.Component {
	constructor(props) {
		super(props);
		const ws = new WebSocket('ws://localhost:8080/');
		ws.onclose = () => alert('Disconnected');
		ws.onmessage = (event) => this.onMessage(JSON.parse(event.data));
		this.state = {
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
		};
	}
	render() {
		return <div>
			<ul>
				{this.state.log.map((m, i) => 
					<li key={i}>{JSON.stringify(m)}</li>
				)}
			</ul>
			{this.state.phase && (playerSideForPhase(this.state.phase) == this.state.player)
				? <button onClick={this.endPhase}>End Phase</button>
				: null}
		</div>;
	}
	onMessage(data) {
		// debug
		this.setState(s => {return {log: s.log.concat([data])}; });
		if (data.method) {
			if (data.method == "error") {
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
	endPhase = () => this.sendInstruction("set_phase", {"phase": nextPhase(this.state.phase)});

	sendInstruction(method, extras) {
		extras.method = method;
		this.state.ws.send(JSON.stringify(extras));
	}
	recvGame(game) {
		this.setState({
			player: game.player,
			phase: game.phase,
		})
	}
};

window.App = App;

