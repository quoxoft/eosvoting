
const CONTRACT_NAME = 'voting'
const LOCAL_TESTNET = 'http://127.0.0.1:8888'
const JUNGLE_TESTNET = 'http://dev.cryptolions.io:38888'
const MAINNET = 'http://mainnet.libertyblock.io:8888'

Eos = require('eosjs')

getCandidates()
getVoters()

function getEos() {
    var privateKey = document.getElementById('private_key').value;
    var config = {
        keyProvider: [privateKey],
        httpEndpoint: JUNGLE_TESTNET,
        broadcast: true,
        sign: true,
        chainId: '038f4b0fc8ff18a4f0842a8f0564611f6e96e8535901dd45e43ac8691a1c4dca',
        expireInSeconds: 60
    }
    return Eos(config);
}

window.vote = function () {
    var eos = getEos();
    var accountName = document.getElementById('eos_account').value;
    var privateKey = document.getElementById('private_key').value;
    var candidateName = document.getElementById('candidate_name').value;

    eos.contract(CONTRACT_NAME).then((contract) => {
        contract.vote(accountName, candidateName, { authorization: accountName })
            .then((res) => {
                document.getElementById('alerts').innerHTML = '<div class="alert alert-success" >Transation Successfully</div>';
            })
            .catch((err) => {
                document.getElementById('alerts').innerHTML = '<div class="alert alert-warning" >There are a problem, Please check your account</div>';;
            })
    })

}




function getCandidates() {
    var eos = getEos();
    var params = {
        json: true,
        scope: CONTRACT_NAME,
        code: CONTRACT_NAME,
        table: "candidates",
        limit: 10
    }
    var tbody = document.querySelector("#block-candidates tbody");
    tbody.innerHTML = '';
    return eos.getTableRows(params).then(resp => {
        var sorted = resp.rows.sort((a, b) => Number(a.total_votes) > Number(b.total_votes) ? -1 : 1);
        sorted.map((candidate, i) =>
            `<tr class="candidate-row">
            <td>${candidate.owner}</td>
            <td>${candidate.total_votes}</td>
            <td>${candidate.candidate_key}</td>
            <td>${candidate.url}</td>
        </tr>`)
            .forEach(row => tbody.innerHTML += row);
    });
}

function getVoters() {
    var eos = getEos();
    var params = {
        json: true,
        scope: CONTRACT_NAME,
        code: CONTRACT_NAME,
        table: "voters",
        limit: 10
    }
    var tbody = document.querySelector("#block-voters tbody");
    tbody.innerHTML = '';
    return eos.getTableRows(params).then(resp => {
        var sorted = resp.rows.sort();
        sorted.map((voter, i) =>
            `<tr class="voter-row">
            <td>${voter.owner}</td>
            <td>${voter.candidates}</td>
            <td>${voter.amount_votes}</td>
        </tr>`)
            .forEach(row => tbody.innerHTML += row);
    });
}

