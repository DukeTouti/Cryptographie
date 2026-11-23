// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract DecentralizedVoting {
	// tout le code du TP va ici
	struct Voter {
		address voterAddress;
		bool hasVoted;
		address votedFor;
		bool hasProposed;
	}

	struct Candidate {
		address candidateAddress;
		uint voteCount;
	}
	
	address public owner;
	bool public votingEnded;

	mapping(address => Voter) public voters;
	mapping(address => Candidate) public candidates;
	address[] public candidateList;
	
	event VoterRegistered(address indexed voter);
	event CandidateProposed(address indexed proposer, address indexed candidate);
	event VoteCast(address indexed voter, address indexed candidate);
	event VotingEnded();

	modifier onlyOwner() {
		require(msg.sender == owner, "Caller is not the owner");
		_;
	}

	modifier onlyRegisteredVoters() {
		require(voters[msg.sender].voterAddress != address(0), "Caller is not a registered voter");
		_;
	}

	modifier onlyBeforeVotingEnded() {
		require(!votingEnded, "Voting has ended");
		_;
	}
	
	constructor() {
		owner = msg.sender;
	}

	function registerVoter(address _voter) public onlyOwner {
		require(_voter != address(0), "Invalid address");
		require(voters[_voter].voterAddress == address(0), "Voter already registered");

		voters[_voter] = Voter({
			voterAddress: _voter,
			hasVoted: false,
			votedFor: address(0),
			hasProposed: false
		});
		
		emit VoterRegistered(_voter);
	}

	function proposeCandidate(address _candidate) public onlyRegisteredVoters onlyBeforeVotingEnded {
		require(_candidate != address(0), "Invalid address");
		require(!voters[msg.sender].hasProposed, "Voter has already proposed a candidate");
		require(candidates[_candidate].candidateAddress == address(0), "Candidate already proposed");

		voters[msg.sender].hasProposed = true;
		candidates[_candidate] = Candidate({candidateAddress: _candidate, voteCount: 0});
		candidateList.push(_candidate);
		
		emit CandidateProposed(msg.sender, _candidate);
	}

	function vote(address _candidate) public onlyRegisteredVoters onlyBeforeVotingEnded {
		require(!voters[msg.sender].hasVoted, "Voter has already voted");
		require(candidates[_candidate].candidateAddress != address(0), "Not a valid candidate");

		voters[msg.sender].hasVoted = true;
		voters[msg.sender].votedFor = _candidate;
		candidates[_candidate].voteCount++;
		
		emit VoteCast(msg.sender, _candidate);
	}

	function endVote() public onlyOwner onlyBeforeVotingEnded {
		votingEnded = true;
		
		emit VotingEnded();
	}

	function getResults() public view returns (address[] memory, uint[] memory) {
		require(votingEnded, "Voting has not ended yet");

		uint n = candidateList.length;
		address[] memory addrs = new address[](n);
		uint[] memory counts = new uint[](n);

		for (uint i = 0; i < n; i++) {
			addrs[i] = candidateList[i];
			counts[i] = candidates[candidateList[i]].voteCount;
		}
		return (addrs, counts);
	}

}











