function [filteredArray ] = lowpassFilter( array, alpha )

%     indEnd = length(array);
%     filteredArray = zeros(indEnd,1);
%     filteredArray(1) = array(1);
%     for i=2:indEnd
%         filteredArray(i) = alpha* array(i) + (1-alpha) * filteredArray(i-1);
%     end 
%     for i=2:indEnd-1
%         filteredArray(i) = 1/4*(array(i-1) + 2*array(i) + array(i+1));
%     end
%         
    order = 3;
    framelen = 21;

    b = sgolay(order,framelen);
    ycenter = conv(array,b((framelen+1)/2,:),'valid');
    ybegin = b(end:-1:(framelen+3)/2,:) * array(framelen:-1:1);
    yend = b((framelen-1)/2:-1:1,:) * array(end:-1:end-(framelen-1));
    filteredArray = [ybegin; ycenter; yend];
end

