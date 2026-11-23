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
}
