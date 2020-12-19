const NULL_SELECTION = {
	selection_state: null,
	selection_hand_index: -1,
	selection_player: null,
	selection_field_segment: null,
	selection_index: -1,
};

class App extends React.Component {
	constructor(props) {
		super(props);
		const ws = new WebSocket('ws://'+window.location.host+'/');
		ws.onopen = () => this.setState({connected: true});
		ws.onclose = () => alert('Disconnected');
		ws.onmessage = (event) => this.onMessage(JSON.parse(event.data));
		this.state = {
			connected: false,
			ws: ws,
			animationInProgress: false,
			nextStates: [],
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
			enemy_junk: [],
			enemy_lost_size: 0,
			selection_state: null,
			selection_hand_index: -1,
			selection_player: null,
			selection_field_segment: null,
			selection_index: -1,
		};
	}
	componentDidMount() {
		window.addEventListener("resize", () => this.forceUpdate());
	}
	render() {
		if (!this.state.connected) {
			return "Waiting for connection to open (if it fails you may be staring at this forever)";
		}
		if (this.state.player == null) {
			return "Connected, waiting for a second player";
		}
		const field_width = window.innerHeight - 150;
		const field_x_offset = (window.innerWidth - field_width)/2;
		const perspective_origin = window.innerHeight*3/4; // fucking hack
		return <div>
			<EnemyHand cards={this.state.enemy_hand_size} />
			<div style={{
				position: "fixed",
				top: 0,
				left: field_x_offset,
				width: field_width + "px",
				height: field_width + "px",
				perspective: perspective_origin + "px",
			}}>
				<Field
					height={field_width}
					player={this.state.player} 
					field={this.state.field}
					junk={this.state.junk}
					deckSize={this.state.deck_size}
					lostSize={this.state.lost_size}
					enemyDeckSize={this.state.enemy_deck_size}
					enemyJunk={this.state.enemy_junk}
					enemyLostSize={this.state.enemy_lost_size}
					selectedPlayer={this.state.selection_player}
					selectedFieldSegment={this.state.selection_field_segment}
					selectedIndex={this.state.selection_index}
					onCardClick={this.onFieldCardClicked}
					rotate={this.props.rotate}
					tilt={this.props.tilt}
				/>
			</div>
			<div style={{
				position: "fixed",
				bottom: 0,
				left: 0,
				width: "100%",
				height: "200px",
				display: 'grid',
				gridTemplateColumns: "150px 300px 1fr",
				gridTemplateRows: '50px 150px',
			}}>
				<div style={{gridColumnStart: 1, gridColumnEnd: 1, gridRowStart: 1, gridRowEnd: 2}}>
					<Phases player={this.state.player} phase={this.state.phase} />
				</div>
				<div style={{gridColumnStart: 2, gridColumnEnd: 2, gridRowStart: 2, gridRowEnd: 2}}>
					<Buttons
						player={this.state.player}
						phase={this.state.phase}
						sendInstruction={this.sendInstruction} />
				</div>
				<div style={{gridColumnStart: 3, gridColumnEnd: 3, gridRowStart: 2, gridRowEnd: 2}}>
					<Hand
						cards={this.state.hand}
						onClick={this.onHandClick}
						selectedIndex={this.state.selection_hand_index}/>
				</div>
			</div>
		</div>;
	}
	onMessage(data) {
		if (data.event == "error") {
			this.recvError(data);
		} else {
			this.recvEvent(data);
		}
	}
	recvError(data) {
		alert(data.error);
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
	recvEvent(game) {
		console.log(game);
		this.setState(s => {
			return {nextStates: s.nextStates.concat([game])};
		}, () => this.startAnimation());
	}
	startAnimation() {
		if (this.state.animationInProgress) {
			return;
		}
		this.setState({animationInProgress: true}, () => {
			const msg = this.state.nextStates[0];
			if (msg.event == "game_over") {
				alert(msg.extras.winner + " wins!");
			} else if (msg.event == "draw_card") {
				animate_draw(msg, this.completeEvent.bind(this));
			} else {
				this.completeEvent()
			}
		});
	}
	completeEvent() {
		this.setState(s => {
			const game = s.nextStates[0];
			return {
				animationInProgress: false,
				nextStates: s.nextStates.slice(1),
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
				enemy_junk: game.enemy_junk,
				enemy_lost_size: game.enemy_lost_size,
			}
		}, () => {
			if (this.state.nextStates.length > 0) {
				this.startAnimation();
			}
		});
	}
	onHandClick = (handIndex) => {
		this.setState(s => {
			if (s.selection_state == "move") {
				// no effect, cancel selection
				return NULL_SELECTION;
			}
			if (s.selection_state == "play") {
				// no effect, cancel selection
				return NULL_SELECTION;
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
		});
	}
	onFieldCardClicked = (player, fs, index) => {
		this.setState(s => {
			if (player == s.selection_player && fs == s.selection_field_segment && index == s.selection_index) {
				// No effect, cancel selection
				return NULL_SELECTION;
			}
			if (s.selection_state == "move") {
				this.sendInstruction("move", {
					start: {
						player_side: s.selection_player,
						field_segment: s.selection_field_segment,
						index: s.selection_index,
					},
					end: {
						player_side: player,
						field_segment: fs,
						index: index,
					}
				})
				return NULL_SELECTION;
			}
			if (s.selection_state == "play") {
				this.sendInstruction("play", {
					hand_index: s.selection_hand_index,
					position: {
						player_side: player,
						field_segment: fs,
						index,
					},
				})
				return NULL_SELECTION;
			}
			return {
				selection_state: "move",
				selection_player: player,
				selection_field_segment: fs,
				selection_index: index,
			};
		});
	}
};

window.App = App;

