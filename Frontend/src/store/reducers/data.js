import * as actionTypes from '../actions/actionTypes';
import { updateObject } from '../utility';
import moment from 'moment';

const initialState = {
	values: {
		temperature: [],
		humidity: [],
		brightness: [],
		times: []
	},
	render: false,
	serverError: false,
	errMessage: null,
	selectedInterval: null,
	loading: false,
	activeDate: null
};

const processResponse = (state, action) => {
	if (action.response.data.data.length <= 0) {
		return updateObject(state, {
			errMessage: 'Žiadne dáta',
			serverError: true,
			render: true,
			loading: false,
			values: {
				temperature: [],
				humidity: [],
				brightness: [],
				times: []
			}
		});
	}

	const temp = [];
	const hum = [];
	const brig = [];
	const tim = [];

	for (let i = action.response.data.data.length - 1; i > 0; i--) {
		const time = moment(action.response.data.data[i].time).format('HH:mm DD/MM');

		temp.unshift(parseFloat(action.response.data.data[i].temperature).toFixed(2));
		hum.unshift(parseFloat(action.response.data.data[i].humidity).toFixed(2));
		brig.unshift(parseFloat(action.response.data.data[i].brightness).toFixed(2));
		tim.unshift(time);
	}

	return updateObject(state, {
		values: {
			temperature: temp,
			humidity: hum,
			brightness: brig,
			times: tim
		},
		render: true,
		serverError: false,
		errMessage: null,
		selectedInterval: action.interval,
		loading: false
	});
};

const throwError = (state, action) => {
	return updateObject(state, {
		serverError: true,
		errMessage: action.error,
		render: true,
		loading: false,
		values: {
			temperature: [],
			humidity: [],
			brightness: [],
			times: []
		}
	});
};

const contactServerStart = (state, action) => {
	return updateObject(state, { loading: true });
};

const changeActiveDate = (state, action) => {
	return updateObject(state, { activeDate: action.date });
};

const reducer = (state = initialState, action) => {
	switch (action.type) {
		case actionTypes.CLEAR_ERROR:
			return updateObject(state, {
				serverError: false,
				errMessage: null
			});
		case actionTypes.CHANGE_ACTIVE_DATE:
			return changeActiveDate(state, action);
		case actionTypes.CONTACT_SERVER_START:
			return contactServerStart(state, action);
		case actionTypes.CONTACT_SERVER_SUCCESS:
			return processResponse(state, action);
		case actionTypes.CONTACT_SERVER_FAIL:
			return throwError(state, action);
		case actionTypes.CLEAR_ACTIVE_VALUES:
			return updateObject(state, {
				values: {
					temperature: [],
					humidity: [],
					brightness: [],
					times: []
				},
				render: false
			});
		default:
			return state;
	}
};

export default reducer;
