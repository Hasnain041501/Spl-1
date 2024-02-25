SELECT DISTINCT S.customer_name from depositor S where not exists ((SELECT branch_name from branch where branch_city='Mirpur' )
minus (SELECT R.branch_name from depositor T, account R where T.account_number=R.account_number and S.customer_name=T.customer_name));
